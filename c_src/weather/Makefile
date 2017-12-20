# Makefile: makefile for weather.c
# 
# make <choice>
# will create a specific climate program.
#
# make
# will create 'england', an English climate program.
#
# make all
# will create all climate programs.
#
# To make addition climate programs, just add the calendar
# and climate choices as -D options.
#
# Calendar choices:
# 
# GREGORIAN - uses essentially modern calendar, without leap years.
#
# JAPAN     - uses medieval Japanese calendar.
#
# MIDDLE_EARTH - uses calendar described in appendices of Tolkien's
#                'The Lord of the Rings'.
#
# Climate choices:
#
# N_ATLANTIC   - useful for Scandinavia coastal or interior continential US
#                or Europe.
# MID_ATLANTIC - useful for England, European coastal, New England, etc.
# S_ATLANTIC   - useful for Spain, Southern coastal US
# GULF         - useful for Gulf States, Mediterranean, etc.
# N_PACIFIC    - useful for Japan, Korea, Southern Alaskan coast, etc.

all: england minnesota japan florida shire georgia

england: weather.c weather.h gregorian.h mid_atlantic.h
	cc -DGREGORIAN -DMID_ATLANTIC -o england -g weather.c

minnesota: weather.c weather.h gregorian.h n_atlantic.h
	cc -DGREGORIAN -DN_ATLANTIC -o minnesota -g weather.c

japan: weather.c weather.h japan.h n_pacific.h
	cc -DJAPAN -DN_PACIFIC -o japan -g weather.c

florida: weather.c weather.h gregorian.h gulf.h
	cc -DGREGORIAN -DGULF -o florida -g weather.c

shire: weather.c weather.h middle_earth.h mid_atlantic.h
	cc -DMIDDLE_EARTH -DMID_ATLANTIC -o shire -g weather.c

georgia: weather.c weather.h gregorian.h s_atlantic.h
	cc -DGREGORIAN -DS_ATLANTIC -o georgia -g weather.c


