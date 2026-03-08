# OpenFOAM PCM Solver

## Overview

This repository contains **customized solvers for OpenFOAM** developed for the simulation of **phase change materials (PCM)** and related heat transfer problems.

The solvers implement the **Porosity–Enthalpy Method** for modeling solid–liquid phase change and include **Gaussian smoothing techniques** to improve numerical stability near the phase interface.

These implementations are intended for research and educational purposes in **computational fluid dynamics (CFD)** and **phase change modeling**.

---

## Features

* Custom OpenFOAM solvers for **PCM phase change simulation**
* Implementation of the **Porosity–Enthalpy Method**
* **Gaussian smoothing kernel** for stabilizing latent heat source terms
* Modified **energy equation formulation**
* Suitable for **solid–liquid phase change problems**
* Designed for **research and numerical experimentation**

---

## Numerical Methods Implemented

### Porosity–Enthalpy Method

The phase change process is modeled using the enthalpy formulation, where the total enthalpy is defined as:

$$
H = h + \Delta H
$$

where

* (h) : sensible heat
* $$(\Delta H) $$: latent heat contribution

The liquid fraction is used to modify the momentum equations via a **porosity-based damping term** to suppress velocity in solid regions.

---

### Gaussian Smoothing for Latent Heat Source

To avoid numerical instability near the phase interface, a **Gaussian smoothing kernel** is applied to the latent heat source term:

$$
S = L \frac{\partial f_l}{\partial t}
$$

where

* (L) : latent heat
* (f_l) : liquid fraction

The Gaussian kernel smooths the phase transition region and improves solver stability.
$$
\gamma = \frac{1}{2} \left[ 1 + \text{erf}\left(\frac{4(T-T_m)}{T_{mr}}\right) \right]
$$

---

## Repositories
I have also add my basic PDE discretization just for the sake of beginners who wish to program in OpenFOAM

## Installation

Clone the repository:

```
git clone https://github.com/veasna-mom/OpenFOAM_Customized_Solvers.git
```

Then compile the solver inside your OpenFOAM environment:

```
wmake
```

---

## Usage

After compiling the solver, run simulations using:

```
yourCustomSolver
```

Example applications include:

* PCM melting/solidification
* Thermal energy storage systems
* Heat transfer with phase change

---

## Requirements

* OpenFOAM
* Linux / WSL environment
* Basic knowledge of CFD and OpenFOAM solvers

---

## Author

Veasna Mom

---

## License

This project is released for **research and educational use**.
