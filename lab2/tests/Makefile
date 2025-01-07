SRC=../my_malloc.c ../printing.c
GCC=gcc
FLAGS = -std=gnu11 -lm -pthread -g

.PHONY: testall
testall: git clean test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12 test13 test14 test15

.PHONY: testPart1
testPart1: git clean test1 test3 test4 test5 test6 test9 test10 test13

.PHONY: test1
test1:
	@${GCC} test1.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 1 && echo "Test 1 \e[92mPASSED\e[0m" || echo "Test 1 \e[91mFAILED\e[0m"
	@${GCC} -m32 test1.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 1-m32 && echo "Test 1-m32 \e[92mPASSED\e[0m" || echo "Test 1-m32 \e[91mFAILED\e[0m"

.PHONY: test2
test2:
	@${GCC} test2.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 2-DefaultArena && echo "Test 2-DefaultArena \e[92mPASSED\e[0m" || echo "Test 2-DefaultArena \e[91mFAILED\e[0m"
	@${GCC} test2.c ${SRC} -DARENA_SIZE=1024 -o test ${FLAGS}
	@bash run_test.sh 2-SmallArena && echo "Test 2-SmallArena \e[92mPASSED\e[0m" || echo "Test 2-SmallArena \e[91mFAILED\e[0m"
	@${GCC} -m32 test2.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 2-DefaultArena-m32 && echo "Test 2-DefaultArena-m32 \e[92mPASSED\e[0m" || echo "Test 2-DefaultArena-m32 \e[91mFAILED\e[0m"
	@${GCC} -m32 test2.c ${SRC} -DARENA_SIZE=1024 -o test ${FLAGS}
	@bash run_test.sh 2-SmallArena-m32 && echo "Test 2-SmallArena-m32 \e[92mPASSED\e[0m" || echo "Test 2-SmallArena-m32 \e[91mFAILED\e[0m"


.PHONY: test3
test3:
	@${GCC} -Wl,--wrap=sbrk test3.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 3 && echo "Test 3 \e[92mPASSED\e[0m" || echo "Test 3 \e[91mFAILED\e[0m"
	@${GCC} -m32 -Wl,--wrap=sbrk test3.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 3-m32 && echo "Test 3-m32 \e[92mPASSED\e[0m" || echo "Test 3-m32 \e[91mFAILED\e[0m"


.PHONY: test4
test4:
	@${GCC} test4.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 4 && echo "Test 4 \e[92mPASSED\e[0m" || echo "Test 4 \e[91mFAILED\e[0m"
	@${GCC} -m32 test4.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 4-m32 && echo "Test 4-m32 \e[92mPASSED\e[0m" || echo "Test 4-m32 \e[91mFAILED\e[0m"

.PHONY: test5
test5:
	@${GCC} test5.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 5 should_fail && echo "Test 5 \e[92mPASSED\e[0m" || echo "Test 5 \e[91mFAILED\e[0m"
	@${GCC} -m32 test5.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 5-m32 should_fail && echo "Test 5-m32 \e[92mPASSED\e[0m" || echo "Test 5-m32 \e[91mFAILED\e[0m"

.PHONY: test6
test6:
	@${GCC} test6.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 6 && echo "Test 6 \e[92mPASSED\e[0m" || echo "Test 6 \e[91mFAILED\e[0m"
	@${GCC} -m32 test6.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 6-m32 && echo "Test 6-m32 \e[92mPASSED\e[0m" || echo "Test 6-m32 \e[91mFAILED\e[0m"

.PHONY: test7
test7:
	@${GCC} test7.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 7 && echo "Test 7 \e[92mPASSED\e[0m" || echo "Test 7 \e[91mFAILED\e[0m"
	@${GCC} -m32 test7.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 7-m32 && echo "Test 7-m32 \e[92mPASSED\e[0m" || echo "Test 7-m32 \e[91mFAILED\e[0m"

.PHONY: test8
test8:
	@${GCC} test8.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 8 && echo "Test 8 \e[92mPASSED\e[0m" || echo "Test 8 \e[91mFAILED\e[0m"
	@${GCC} -m32 test8.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 8-m32 && echo "Test 8-m32 \e[92mPASSED\e[0m" || echo "Test 8-m32 \e[91mFAILED\e[0m"

.PHONY: test9
test9:
	@${GCC} test9.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 9 && echo "Test 9 \e[92mPASSED\e[0m" || echo "Test 9 \e[91mFAILED\e[0m"
	@${GCC} -m32 test9.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 9-m32 && echo "Test 9-m32 \e[92mPASSED\e[0m" || echo "Test 9-m32 \e[91mFAILED\e[0m"

.PHONY: test10
test10:
	@${GCC} test10.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 10 && echo "Test 10 \e[92mPASSED\e[0m" || echo "Test 10 \e[91mFAILED\e[0m"
	@${GCC} -m32 test10.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 10-m32 && echo "Test 10-m32 \e[92mPASSED\e[0m" || echo "Test 10-m32 \e[91mFAILED\e[0m"

.PHONY: test11
test11:
	@${GCC} test11.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 11 && echo "Test 11 \e[92mPASSED\e[0m" || echo "Test 11 \e[91mFAILED\e[0m"
	@${GCC} -m32 test11.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 11-m32 && echo "Test 11-m32 \e[92mPASSED\e[0m" || echo "Test 11-m32 \e[91mFAILED\e[0m"

.PHONY: test12
test12:
	@${GCC} -Wl,--wrap=sbrk test12.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 12 && echo "Test 12 \e[92mPASSED\e[0m" || echo "Test 12 \e[91mFAILED\e[0m"
	@${GCC} -m32 -Wl,--wrap=sbrk test12.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 12-m32 && echo "Test 12-m32 \e[92mPASSED\e[0m" || echo "Test 12-m32 \e[91mFAILED\e[0m"

.PHONY: test13
test13:
	@${GCC} test13.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 13 && echo "Test 13 \e[92mPASSED\e[0m" || echo "Test 13 \e[91mFAILED\e[0m"
	@${GCC} -m32 test13.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 13-m32 && echo "Test 13-m32 \e[92mPASSED\e[0m" || echo "Test 13-m32 \e[91mFAILED\e[0m"

.PHONY: test14
test14:
	@${GCC} test14.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 14-DefaultArena && echo "Test 14-DefaultArena \e[92mPASSED\e[0m" || echo "Test 14-DefaultArena \e[91mFAILED\e[0m"
	@${GCC} test14.c ${SRC} -DARENA_SIZE=1024 -o test ${FLAGS}
	@bash run_test.sh 14-SmallArena && echo "Test 14-SmallArena \e[92mPASSED\e[0m" || echo "Test 14-SmallArena \e[91mFAILED\e[0m"
	@${GCC} -m32 test14.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 14-DefaultArena-m32 && echo "Test 14-DefaultArena-m32 \e[92mPASSED\e[0m" || echo "Test 14-DefaultArena-m32 \e[91mFAILED\e[0m"
	@${GCC} -m32 test14.c ${SRC} -DARENA_SIZE=1024 -o test ${FLAGS}
	@bash run_test.sh 14-SmallArena-m32 && echo "Test 14-SmallArena-m32 \e[92mPASSED\e[0m" || echo "Test 14-SmallArena-m32 \e[91mFAILED\e[0m"
.PHONY: test15
test15:
	@${GCC} test15.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 15-DefaultArena && echo "Test 15-DefaultArena \e[92mPASSED\e[0m" || echo "Test 15-DefaultArena \e[91mFAILED\e[0m"
	@${GCC} test15.c ${SRC} -DARENA_SIZE=1024 -o test ${FLAGS}
	@bash run_test.sh 15-SmallArena && echo "Test 15-SmallArena \e[92mPASSED\e[0m" || echo "Test 15-SmallArena \e[91mFAILED\e[0m"
	@${GCC} -m32 test15.c ${SRC} -o test ${FLAGS}
	@bash run_test.sh 15-DefaultArena-m32 && echo "Test 15-DefaultArena-m32 \e[92mPASSED\e[0m" || echo "Test 15-DefaultArena-m32 \e[91mFAILED\e[0m"
	@${GCC} -m32 test15.c ${SRC} -DARENA_SIZE=1024 -o test ${FLAGS}
	@bash run_test.sh 15-SmallArena-m32 && echo "Test 15-SmallArena-m32 \e[92mPASSED\e[0m" || echo "Test 15-SmallArena-m32 \e[91mFAILED\e[0m"

.PHONY: clean
clean:
	rm -f *.o test log.txt output/*

.PHONY: git
git:
	(cd .. && make)

