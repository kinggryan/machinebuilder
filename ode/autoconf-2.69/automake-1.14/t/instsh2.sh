#! /bin/sh
# Copyright (C) 2002-2013 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Various install-sh checks.

am_create_testdir=empty
. test-init.sh

get_shell_script install-sh

# Basic errors.
./install-sh && exit 1
./install-sh -m 644 dest && exit 1

# Directories.

# It should be OK to create no directory.  We sometimes need
# this when directory are conditionally defined.
./install-sh -d
# One directory.
./install-sh -d d0
test -d d0
# Multiple directories (for make installdirs).
./install-sh -d d1 d2 d3 d4
test -d d1
test -d d2
test -d d3
test -d d4
# Subdirectories.
./install-sh -d p1/p2/p3 p4//p5//p6//
test -d p1/p2/p3
test -d p4/p5/p6

# Files.
: > x
./install-sh -c -m 644 x y
test -f x
test -f y
./install-sh -m 644 y z
test -f y
test -f z
# Multiple files.
./install-sh -m 644 -c x z d1
test -f x
test -f z
test -f d1/x
test -f d1/z
./install-sh -m 644 x z d2//
test -f x
test -f z
test -f d2/x
test -f d2/z
./install-sh -t d3 -m 644 x z
test -f x
test -f z
test -f d3/x
test -f d3/z
./install-sh -t d4// -m 644 x z
test -f x
test -f z
test -f d4/x
test -f d4/z
./install-sh -T x d3/y
test -f x
test -f d3/y
./install-sh -T x d3 && exit 1
./install-sh -T x d4// && exit 1

# Ensure that install-sh works with names that include spaces.
touch 'a  b'
mkdir 'x  y'
./install-sh 'a  b' 'x  y'
test -f x\ \ y/a\ \ b
test -f 'a  b'

# Ensure we do not run into 'test' operator precedence bugs with Tru64 sh.
for c in = '(' ')' '!'; do
  ./install-sh $c 2>stderr && { cat stderr >&2; exit 1; }
  cat stderr >&2
  grep 'test: ' stderr && exit 1
  # Skip tests if the file system is not capable.
  mkdir ./$c || continue
  rmdir ./$c
  ./install-sh -d $c/$c/$c
  rm -rf ./$c
  ./install-sh -d $c d5/$c/$c
  test -d ./$c
  test -d d5/$c/$c
  ./install-sh x $c
  test -f ./$c/x
  rm -f ./$c/x
  ./install-sh -t $c x
  test -f ./$c/x
  rm -rf ./$c
  ( : > ./$c ) || continue
  ./install-sh $c x d5/$c/$c
  test -f d5/$c/$c/x
  test -f d5/$c/$c/$c
  rm -f d5/$c/$c/?
  ./install-sh -t d5/$c/$c $c x
  test -f d5/$c/$c/x
  test -f d5/$c/$c/$c
done

:
