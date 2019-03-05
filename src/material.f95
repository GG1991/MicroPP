!
! This file is part of the Fortran_C++ program.
! Copyright (c) 2018 Jimmy Aguilar Mena.
!
! This program is free software: you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation, version 3.
!
! This program is distributed in the hope that it will be useful, but
! WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
! General Public License for more details.
!
! You should have received a copy of the GNU General Public License
! along with this program. If not, see <http://www.gnu.org/licenses/>.
!

module libmaterial
  use iso_c_binding

  implicit none

  ! If you change this remember to change also the material_base C struct
  type, bind(C) :: material_base
     real(c_double) :: E, nu, Ka, Sy
     real(c_double) :: k, mu, lambda
     integer(c_int) type
     logical(c_bool) plasticity, damage
  end type material_base

  interface
     subroutine material_set(this, E, nu, Ka, Sy, type) &
          bind (C, name='material_set')
       use, intrinsic :: iso_c_binding, only: c_int, c_float
       import material_base
       implicit none
       type(material_base) :: this
       real(c_float), intent(in), value :: E, nu, Ka, Sy
       integer(c_int), intent(in), value :: type
     end subroutine material_set
  end interface

end module libmaterial
