# Assignment 3

## Installation

type 'make' on unix, and 'make OS=windows' on windows

## Execution

type the following command to run the program

```
./assignment3 file.emitter
```

some emitters are pre-defined. For example:

```
./assignment3 ./emitters/snow.emitter
./assignment3 ./emitters/explosion.emitter
./assignment3 ./emitters/fire.emitter
./assignment3 ./emitters/waboom.emitter
```

## Emitter file

emitter files define how the particles should move and animate. Many parameters can be defined in this file. Some parameters can be split into a min/max pair. Some of theses min/max pairs animate over a particles lifetime. The min/max pairs are specified by prefixing the variable with 'min_' or 'max_'.

| Variable | type | has min/max | description |
| --- | --- | --- | --- |
| vel | float | yes | initial velocity of particle |
| dir | Vec3 | yes | initial direction of particle |
| offset | Vec3 | yes | offset of initial position from emitter |
| life | float | yes | lifetime of particle |
| col | Vec3 | yes | colour of particle |
| size | float | yes | size of particle |
| grav | Vec3 | no | gravity of system |
| wind | Vec3 | no | wind velocity of system |
| drag | float | no | drag coefficient of particles |
| mass | float | no | mass of particles |
| vortex_axis | Vec3 | yes | axis of rotation for vortices |
| vortex_angular_vel | float | yes | angular velocity of vortices |
| delay | float | yes | time delay between particle emission |
| vortex_delay | float | yes | time delay between vortex emission |
| max | int | no | maximum number of particles |
| vortex_max | int | no | maximum number of vortices |
| tex | string int int | no | texture file, number of images, images per row |

see one of the pre-defined emitter files for examples

## Contributors

[Eric Roberts](https://github.com/E-Rockalanche)