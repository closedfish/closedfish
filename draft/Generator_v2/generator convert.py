#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jan 17 20:59:47 2023

@author: ariel.flahaut
"""

def remove_empty_lines(file_name):
    with open(file_name) as f:
        lines = f.readlines()
    non_empty_lines = [line for line in lines if line.strip() != '']
    with open(file_name, 'w') as f:
        f.write(''.join(non_empty_lines))

def flip_lines(file_name):
    with open(file_name) as f:
        lines = f.readlines()
    flipped_lines = []
    for i in range(0, len(lines), 2):
        flipped_lines.append(lines[i+1])
        flipped_lines.append(lines[i])
    with open(file_name, 'w') as f:
        f.writelines(flipped_lines)
def add_space(file_name):
    with open(file_name) as f:
        lines = f.readlines()
    spaced_lines = []
    for i in range(0, len(lines), 2):
        spaced_lines.append(lines[i])
        spaced_lines.append(lines[i+1])
        spaced_lines.append('\n')
    with open(file_name, 'w') as f:
        f.writelines(spaced_lines)
