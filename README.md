# rpi-arcade
A baremetal raspberry pi arcade machine! This repo details an arcade machine built to run baremetal on a Raspberry Pi Model 1A. 

## Details

This project is built to run a rudimentary arcade machine on a Raspberry Pi A+ on Baremetal (No OS, just a bootloader and raw, dangerous, unprotected code). The top level directory contains the arcade machine functionality. The directories `flappy_base` and `pacman_base` contain the original versions of their respective games. 

## Motivation

I took CS 107E as a Freshman at Stanford and I was really excited and LOST by all the material. I've now been a CA for 107E for two quarters, and I wanted to take some time to work through a project to practice some more baremetal skills.

## Baremetal

Running code on bare metal is as low as you can program on any hardware stack, before running into actual hardware elements. In CS 107E, we don't use any external libraries for the majority of our assignments, but we provide a decent amount of library code for complicated tasks like the bootloader, interrupts, and the framebuffer. In this project, I utilize the libraries from the class for system configuration and some interrupts.

## Demos

Check out a demo and some pictures !(Here)[https://drive.google.com/drive/folders/1oKguVy4MbEjsR6Ve_sOsZHEdrnCE-JZL?usp=sharing]

## Contributions
This project uses material from the Stanford CS 107E course repository found [here](https://github.com/cs107e/cs107e.github.io). The code used in the libpi and libpiextra libraries are not mine, and were written by the CS 107E course staff over the years. Specific contributions can be found in the header files available on the course repo. Additionally, the code for the "Flap-pi Bird" (popularly known as "Flappy Bird") and Pi-man (popularly known as "pacman") was written as a part of a joint final project for the CS 107E class between Claire Huang, Matthew Tan, and Sean Konz (me). The project has been heavily adapted from it's initial version.
