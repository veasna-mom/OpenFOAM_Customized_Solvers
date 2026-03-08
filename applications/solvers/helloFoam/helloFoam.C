/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2025 AUTHOR,AFFILIATION
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    helloFoam

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
using namespace Foam;  // this is just like std

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{
    //#include "setRootCase.H"
    //#include "createTime.H" // thi use to write time cases like 0 1 ...

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
     //Info = printf in C++
    Info<< "Hello This is First OpenFOAM Code \nEnd\n" << endl;
	label a = 20;
	label b = 30; //Integer in C++
	scalar x = 30.0; // Double in C++
	Info<< " Sum of a and b="<<a*b<<endl;
	Info << "Sin of x =" <<Foam::sin(x)<<endl; // Just like std::sin(x)
	Info << " X^3 is "<<Foam::pow(x,3)<<endl;
	
    return 0;
}


// ************************************************************************* //
