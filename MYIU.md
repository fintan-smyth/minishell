TODO


-ctrl \ after typing -> quit
echo with  " "/ '$USER'

-env dont have enough info
	I added char *env[] in main, we can just printf

-exit



-multi hdoc(when close, leak fd from pipe)

==1325950== FILE DESCRIPTORS: 6 open (3 std) at exit.
==1325950== Open file descriptor 5:
==1325950==    at 0x49DA07D: pipe (pipe.c:29)
==1325950==    by 0x4055FB: read_hdoc (redirect_actions.c:127)
==1325950==    by 0x405884: redirect_hdoc (redirect_actions.c:188)
==1325950==    by 0x4051FC: apply_redirection (redirect.c:66)
==1325950==    by 0x403C46: parse_pipeline (parse_line.c:75)
==1325950==    by 0x403F9E: traverse_ptree (parse_tree.c:69)
==1325950==    by 0x403D1E: parse_line (parse_line.c:95)
==1325950==    by 0x40166D: main (main.c:97)
==1325950== 
==1325950== Open file descriptor 4:
==1325950==    at 0x49DA07D: pipe (pipe.c:29)
==1325950==    by 0x4055FB: read_hdoc (redirect_actions.c:127)
==1325950==    by 0x405884: redirect_hdoc (redirect_actions.c:188)
==1325950==    by 0x4051FC: apply_redirection (redirect.c:66)
==1325950==    by 0x403C46: parse_pipeline (parse_line.c:75)
==1325950==    by 0x403F9E: traverse_ptree (parse_tree.c:69)
==1325950==    by 0x403D1E: parse_line (parse_line.c:95)
==1325950==    by 0x40166D: main (main.c:97)
==1325950== 
==1325950== Open file descriptor 3:
==1325950==    at 0x49DA07D: pipe (pipe.c:29)
==1325950==    by 0x4055FB: read_hdoc (redirect_actions.c:127)
==1325950==    by 0x405884: redirect_hdoc (redirect_actions.c:188)
==1325950==    by 0x4051FC: apply_redirection (redirect.c:66)
==1325950==    by 0x403C46: parse_pipeline (parse_line.c:75)
==1325950==    by 0x403F9E: traverse_ptree (parse_tree.c:69)
==1325950==    by 0x403D1E: parse_line (parse_line.c:95)
==1325950==    by 0x40166D: main (main.c:97)












to keep updated
	git fetch upsteam
	git merge upsteam/main
	or
	git rebase upsteam/main

git push upsteam myiu

git push origin myiu 

