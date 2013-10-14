#!/bin/sh

awk ' /remaining*/ {print (100 * $3 / 7200)"%";}' /proc/acpi/battery/BAT1/state
