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

  implicit none

  private :: new_micropp3, set_macro_strain, homogenize, &
       get_macro_stress, get_macro_ctan, update_vars, &
       get_nl_flag, output

  public :: micropp3, free

  type :: micropp3
     private
     integer(8) :: ptr ! pointer
   contains
     procedure :: set_macro_strain, homogenize, &
          get_macro_stress, get_macro_ctan, update_vars, &
          get_nl_flag, get_cost, has_converged, output, print_info
  end type micropp3

  interface micropp3
     procedure new_micropp3
  end interface micropp3

  integer(8) :: init3
  external init3

contains

  !------------------------------------------------------------
  ! Constructors

  function new_micropp3(ngp, size, micro_type, micro_params, params)
    implicit none
    type(micropp3) :: new_micropp3
    integer, intent(in) :: ngp
    integer, intent(in) :: size(3)
    integer, intent(in) :: micro_type
    real(8), intent(in), dimension (*) :: micro_params
    type(material_t), intent(in), dimension (*) :: params

    new_micropp3%ptr = init3(ngp, size, micro_type, micro_params, params)

  end function new_micropp3

  subroutine free(this)
    class(micropp3) :: this
    call free3(this%ptr)
  end subroutine free

  !------------------------------------------------------------
  ! Main Calculation

  subroutine set_macro_strain(this, gp_id, macro_strain)
    class(micropp3), intent(inout) :: this
    integer, intent(in) :: gp_id
    real(8), intent(in), dimension(*) :: macro_strain
    call set_macro_strain3(this%ptr, gp_id, macro_strain)
  end subroutine set_macro_strain

  subroutine homogenize(this)
    class(micropp3) :: this
    call homogenize3(this%ptr)
  end subroutine homogenize

  subroutine get_macro_stress(this, gp_id, macro_stress)
    class(micropp3) :: this
    integer, intent(in) :: gp_id
    real(8), intent(out), dimension(*) :: macro_stress
    call get_macro_stress3(this%ptr, gp_id, macro_stress)
  end subroutine get_macro_stress

  subroutine get_macro_ctan(this, gp_id, macro_ctan)
    class(micropp3) :: this
    integer, intent(in) :: gp_id
    real(8), intent(out), dimension(*) :: macro_ctan
    call get_macro_ctan3(this%ptr, gp_id, macro_ctan)
  end subroutine get_macro_ctan

  subroutine update_vars(this)
    class(micropp3) :: this
    call update_vars3(this%ptr)
  end subroutine update_vars

  !------------------------------------------------------------
  ! Output

  subroutine get_nl_flag(this, gp_id, nl_flag)
    implicit none
    class(micropp3), intent(inout) :: this
    integer, intent(in) :: gp_id
    integer, intent(out) :: nl_flag
    call get_nl_flag3(this%ptr, gp_id, nl_flag)
  end subroutine

  subroutine get_cost(this, gp_id, cost)
    implicit none
    class(micropp3), intent(in) :: this
    integer, intent(in) :: gp_id
    integer, intent(out) :: cost
    call get_cost3(this%ptr, gp_id, cost)
  end subroutine

  subroutine has_converged(this, gp_id, converged)
    implicit none
    class(micropp3), intent(in) :: this
    integer, intent(in) :: gp_id
    logical, intent(out) :: converged
    call has_converged3(this%ptr, gp_id, converged)
  end subroutine

  subroutine print_info(this)
    class(micropp3) :: this
    call print_info3(this%ptr)
  end subroutine print_info

  subroutine output(this, gp_id, filename)
    class(micropp3) :: this
    integer, intent(in) :: gp_id
    Character(128), intent(in) :: filename
    call output3(this%ptr, gp_id, filename)
  end subroutine output

end module libmicropp
