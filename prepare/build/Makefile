WARN = -Wall -Wextra -Wconversion -Werror
SANITIZE = -fsanitize=address,undefined


prepare_machine_from_log:
	g++-11 $(WARN) $(SANITIZE) -O3 ../prepare_machine_from_log.cpp ../../LogReader.cpp -o prepare_machine_from_log.out

prepare_job_and_task_from_log:
	g++-11 $(WARN) $(SANITIZE) -O3 ../prepare_job_and_task_from_log.cpp ../../LogReader.cpp -o prepare_job_and_task_from_log.out
