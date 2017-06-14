#!/bin/bash

make && bin/simulation < status_in.txt > henry_out.txt; meld henry_out.txt status_out.txt;
