#!/bin/bash

MS_DEBUG=$1 valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=readline.supp -s ./minishell
