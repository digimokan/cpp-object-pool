################################################################################
# REQUIRED COMMAND-LINE INPUTS
#   1) -DBUILD_TESTING=[ON/OFF]
#   2) -DCMAKE_BUILD_TYPE=[Debug/Release/RelWithDebInfo/MinSizeRel]
#   3) -Dproject_config_file=[relative/path/to/config_settings_file]
#   4) -Dproject_source_main_exec=[source_file_name]
# OPTIONAL COMMAND-LINE INPUTS
#   1) -Dproject_main_exec_to_build=[executable_name]
#   2) -Dtesting_exec_to_build=[executable_name]
################################################################################

################################################################################
# MACROS / FUNCTIONS
################################################################################

function(include_dir_and_recurse_subdirs)
  foreach(arg IN LISTS ARGN)
    include_directories("${arg}")
    file(GLOB_RECURSE subdir_list LIST_DIRECTORIES true RELATIVE "${PROJECT_SOURCE_DIR}/" CONFIGURE_DEPENDS "${arg}/*")
    foreach(sdir ${subdir_list})
      if(IS_DIRECTORY "${PROJECT_SOURCE_DIR}/${sdir}")
        include_directories("${sdir}")
      endif()
    endforeach()
  endforeach()
endfunction()

function(collect_source_files_recursively out_sources)
  set(collected_sources "")
  foreach(arg IN LISTS ARGN)
    file(GLOB_RECURSE src CONFIGURE_DEPENDS "${arg}/*.c" "${arg}/*.cpp" "${arg}/*.hxx")
    list(APPEND collected_sources ${src})
  endforeach()
  set(${out_sources} ${collected_sources} PARENT_SCOPE)
endfunction()

function(convert_lang_type_field out_lang_type)
  if(${${out_lang_type}} STREQUAL "C")
    set(out_lang_type "C" PARENT_SCOPE)
  elseif(${${out_lang_type}} STREQUAL "CPP")
    set(${out_lang_type} "CXX" PARENT_SCOPE)
  else()
    message(FATAL_ERROR "Reading from project config file: unable to read/convert project_language_type")
  endif()
endfunction()

function(read_config_list_field out_field out_config_list)
  list(REMOVE_AT ${out_config_list} 0)
  list(GET ${out_config_list} 0 line)
  list(FIND ARGN "field_is_list" index)
  if(${index} GREATER -1)
    string(REPLACE ":" ";" line "${line}")
  endif()
  list(FIND ARGN "field_is_optional" index)
  if((${index} GREATER -1) AND ("${line}" STREQUAL "-"))
    set(line "")
  endif()
  list(FIND ARGN "field_is_lang_type" index)
  if(${index} GREATER -1)
    convert_lang_type_field(line)
  endif()
  set(${out_field} "${line}" PARENT_SCOPE)
  list(REMOVE_AT ${out_config_list} 0)
  set(${out_config_list} ${${out_config_list}} PARENT_SCOPE)
endfunction()

function(skip_config_list_field out_config_list)
  list(REMOVE_AT ${out_config_list} 0)
  list(REMOVE_AT ${out_config_list} 0)
  set(${out_config_list} ${${out_config_list}} PARENT_SCOPE)
endfunction()

################################################################################
# READ SETTINGS FROM PROJECT CONFIG FILE
################################################################################

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${project_config_file}")
file(STRINGS "${project_config_file}" project_config_list)
read_config_list_field(project_cmake_title project_config_list)
read_config_list_field(project_cmake_version_req project_config_list)
read_config_list_field(project_language_type project_config_list "field_is_lang_type")
read_config_list_field(project_language_standard project_config_list)
read_config_list_field(project_include_standards project_config_list "field_is_optional")
read_config_list_field(project_warning_levels project_config_list "field_is_optional")
read_config_list_field(project_source_dirs project_config_list "field_is_list")
skip_config_list_field(project_config_list)
skip_config_list_field(project_config_list)
read_config_list_field(testing_language_type project_config_list "field_is_lang_type")
read_config_list_field(testing_language_standard project_config_list)
read_config_list_field(testing_source_dirs project_config_list "field_is_list")
skip_config_list_field(project_config_list)

################################################################################
# CMAKE VERSION REQUIRED
################################################################################

cmake_minimum_required(VERSION "${project_cmake_version_req}")

################################################################################
# MAIN PROJECT DEF
################################################################################

project("${project_cmake_title}" NONE)

################################################################################
# CMAKE LANGUAGE SUPPORT CHECK
################################################################################

enable_language("${project_language_type}")

################################################################################
# SOURCE FILE GROUPS
################################################################################

# project sources
set(project_sources "")
collect_source_files_recursively(project_sources ${project_source_dirs})

################################################################################
# EXECUTABLES TO BUILD
################################################################################

if(NOT "${project_source_main_exec}" STREQUAL "-")
  # project main entry point source
  set(project_main_source "${project_source_main_exec}")

  # executable file to produce for main source
  add_executable("${project_main_exec_to_build}" "${project_source_main_exec}")

  # remove main source from project_sources
  list(REMOVE_ITEM project_sources "${CMAKE_CURRENT_SOURCE_DIR}/${project_source_main_exec}")
endif()

################################################################################
# USER LIB (ALL NON-MAIN SOURCES COLLECTED INTO ONE LIB)
################################################################################

add_library(user_lib ${project_sources})

################################################################################
# EXTERNAL LIBS
################################################################################

find_package(Threads REQUIRED)

################################################################################
# LINK LIBS --> EXECUTABLES
################################################################################

if(NOT "${project_source_main_exec}" STREQUAL "-")
  target_link_libraries("${project_main_exec_to_build}" PRIVATE user_lib PRIVATE Threads::Threads)
endif()

################################################################################
# COMPILER OPTIONS
################################################################################

# warnings: enable most warnings
add_definitions("${project_warning_levels}")

################################################################################
# SOURCE CONFIG
################################################################################

# set project-wide language standards
if(NOT "${project_source_main_exec}" STREQUAL "-")
  set_target_properties("${project_main_exec_to_build}" PROPERTIES ${project_language_type}_STANDARD ${project_language_standard} ${project_language_type}_STANDARD_REQUIRED ON)
endif()
set_target_properties(user_lib PROPERTIES ${project_language_type}_STANDARD ${project_language_standard} ${project_language_type}_STANDARD_REQUIRED ON)

# set project-wide system-include standards
add_definitions("${project_include_standards}")

# set project-wide include dirs
include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_dir_and_recurse_subdirs(${project_source_dirs})

################################################################################
# TESTING (OPTIONAL ADD-ON TO BUILD)
################################################################################

# note BUILD_TESTING=ON must be passed to cmake (auto-calls enable_testing())
if(BUILD_TESTING)

  # load CTest suite build tests
  include(CTest)

  # cmake language support check
  enable_language("${testing_language_type}")

  # source file lists
  set(testing_sources "")
  collect_source_files_recursively(testing_sources ${testing_source_dirs})

  # test harness executable
  add_executable("${testing_exec_to_build}" ${testing_sources})
  # cmake ctests to run / linked libs
  add_test(NAME "${testing_exec_to_build}" COMMAND "${testing_exec_to_build}")
  # link libs --> executables
  target_link_libraries("${testing_exec_to_build}" PRIVATE user_lib PRIVATE Threads::Threads)

  # source config

  # testing language standards
  set_target_properties("${testing_exec_to_build}" PROPERTIES
    ${testing_language_type}_STANDARD "${testing_language_standard}"
    ${testing_language_type}_STANDARD_REQUIRED ON)

  # testing include dirs
  include_dir_and_recurse_subdirs(${testing_source_dirs})

endif()

