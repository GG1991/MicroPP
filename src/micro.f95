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

module libmicropp
  use libmaterial
  use iso_c_binding

  implicit none

  ! Important: if you change this remember to change also the equivalent
  ! one in the C_wrapper
  type, bind(C) :: micropp3
     type(c_ptr) :: ptr ! pointer
  end type micropp3

  interface
     subroutine micropp3_new(self, ngp, size, micro_type, micro_params, params) &
          bind(C)
       use, intrinsic :: iso_c_binding, only: c_int, c_double
       use libmaterial
       import micropp3
       implicit none
       type(micropp3), intent(out) :: self
       integer(c_int), value :: ngp
       integer(c_int), intent(in) :: size(3)
       integer(c_int), value :: micro_type
       real(c_double), intent(in), dimension (*) :: micro_params
       type(material_base), intent(in), dimension (*) :: params
     end subroutine micropp3_new

     subroutine micropp3_free(this) bind(C)
       import micropp3
       implicit none
       type(micropp3), intent(inout) :: this
     end subroutine micropp3_free

     subroutine micropp3_set_macro_strain(this, gp_id, macro_strain) bind(C)
       use, intrinsic :: iso_c_binding, only: c_int, c_double
       import micropp3
       implicit none
       type(micropp3), intent(inout) :: this
       integer(c_int), intent(in), value :: gp_id
       real(c_double), intent(in), dimension(*) :: macro_strain
     end subroutine micropp3_set_macro_strain

     subroutine micropp3_homogenize(this) bind(C)
       import micropp3
       implicit none
       type(micropp3), intent(inout) :: this
     end subroutine micropp3_homogenize

     logical(c_bool) function micropp3_get_nl_flag(this, gp_id) bind(C)
       use, intrinsic :: iso_c_binding, only: c_bool, c_int
       import micropp3
       implicit none
       type(micropp3), intent(in) :: this
       integer(c_int), intent(in), value :: gp_id
     end function micropp3_get_nl_flag

     integer(c_int) function micropp3_get_cost(this, gp_id) bind(C)
       use, intrinsic :: iso_c_binding, only: c_int
       import micropp3
       implicit none
       type(micropp3), intent(in) :: this
       integer(c_int), intent(in), value :: gp_id
     end function micropp3_get_cost

     subroutine micropp3_get_macro_stress(this, gp_id, macro_stress) bind(C)
       use, intrinsic :: iso_c_binding, only: c_int, c_double
       import micropp3
       implicit none
       type(micropp3), intent(inout) :: this
       integer(c_int), intent(in), value :: gp_id
       real(c_double), intent(in), dimension(*) :: macro_stress
     end subroutine micropp3_get_macro_stress

     subroutine micropp3_get_macro_ctan(this, gp_id, macro_stress) bind(C)
       use, intrinsic :: iso_c_binding, only: c_int, c_double
       import micropp3
       implicit none
       type(micropp3), intent(inout) :: this
       integer(c_int), intent(in), value :: gp_id
       real(c_double), intent(in), dimension(*) :: macro_stress
     end subroutine micropp3_get_macro_ctan

     subroutine micropp3_update_vars(this) bind(C)
       import micropp3
       implicit none
       type(micropp3), intent(inout) :: this
     end subroutine micropp3_update_vars

     subroutine micropp3_output(this, gp_id, filename) bind(C)
       use, intrinsic :: iso_c_binding, only: c_int, c_char
       import micropp3
       implicit none
       type(micropp3), intent(inout) :: this
       integer(c_int), intent(in), value :: gp_id
       character(kind=c_char), intent(in) :: filename(128)
     end subroutine micropp3_output

     subroutine micropp3_print_info(this) bind(C)
       import micropp3
       implicit none
       type(micropp3), intent(in) :: this
     end subroutine micropp3_print_info

  end interface

end module libmicropp
