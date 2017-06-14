#!/bin/bash

make && bin/simulation < show_in.txt > henry_out.txt; meld henry_out.txt show_out.txt;
