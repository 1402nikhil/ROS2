# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_course_activities_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED course_activities_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(course_activities_FOUND FALSE)
  elseif(NOT course_activities_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(course_activities_FOUND FALSE)
  endif()
  return()
endif()
set(_course_activities_CONFIG_INCLUDED TRUE)

# output package information
if(NOT course_activities_FIND_QUIETLY)
  message(STATUS "Found course_activities: 0.0.0 (${course_activities_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'course_activities' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${course_activities_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(course_activities_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${course_activities_DIR}/${_extra}")
endforeach()
