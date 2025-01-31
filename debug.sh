#!/bin/bash

MS_PARSE_DEBUG=ON valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=readline.supp -s ./minishell
