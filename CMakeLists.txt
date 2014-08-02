## Copyright 2014 Pavlihin Ilya
# This file is part of mcabber module writing howto examples.
#
# Examples are free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.6) 
project(mnotify C) 
set(PROJECT_VERSION "0.0.1")

## User settable options
set(OPT_MNOTIFY_HEADER          "mnotify_header"          CACHE STRING "Name for mcabber option to enable/disable header with command line")
set(OPT_MNOTIFY_HEADER_INLINE   "mnotify_header_inline"   CACHE STRING "Name for mcabber option to switch between headline/inline header")
set(OPT_MNOTIFY_REDIRECT_STDERR "mnotify_redirect_stderr" CACHE STRING "Name for mcabber option to enable stderr capturing")
set(OPT_MNOTIFY_SHELL           "mnotify_shell"           CACHE STRING "Name for mcabber option to specify shell to be used to execute command")

## Check for build dependencies
find_package(PkgConfig REQUIRED) 
pkg_check_modules(GLIB REQUIRED glib-2.0) 
pkg_check_modules(MCABBER REQUIRED mcabber)
pkg_check_modules(GTK+ REQUIRED gtk+-2.0)
pkg_check_modules(LIBNOTIFY REQUIRED libnotify)

link_directories(${GLIB_LIBRARY_DIRS}
				 ${MCABBER_LIBRARY_DIRS}
				 ${GTK+_LIBRARY_DIRS}
				 ${LIBNOTIFY_LIBRARY_DIRS})

## Target definitions
add_library(mnotify MODULE mnotify.c) 

## Compiler setup
#configure_file(config.h.in config.h)
include_directories(SYSTEM ${GLIB_INCLUDE_DIRS} 
  ${MCABBER_INCLUDE_DIRS} ${GTK+_INCLUDE_DIRS} ${LIBNOTIFY_INCLIDE_DIRS})
target_link_libraries(mnotify ${GLIB_LIBRARIES} 
  ${MCABBER_LIBRARIES} ${GTK+_LIBRARIES} ${LIBNOTIFY_LIBRARIES})
set_target_properties(mnotify PROPERTIES COMPILE_FLAGS "-Wall")

## The End ## vim: se ts=4: ##
