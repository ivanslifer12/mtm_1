#TESTFL=exampleMain
#TESTFL=test_fb1_main
#TESTFL=mtmElections_test
#TESTFL=TestsFinal


CC=gcc
OBJS=uniqueOrderedList/uniqueOrderedList.o preference.o citizen.o city.o mtmElecions.o candidate.o
EXEC=ex1
DEBUG=-g  #-g for debug
CFLAGS=-std=c99 -Wall -Werror -pedantic-errors $(DEBUG)

$(EXEC)  : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

uniqueOrderedList.o: uniqueOrderedList/uniqueOrderedList.c  uniqueOrderedList/uniqueOrderedList.h
preference.o: preference.c preference.h
citizen.o: citizen.c citizen.h  preference.h uniqueOrderedList/uniqueOrderedList.h
candidate.o : candidate.c candidate.h
city.o: city.c city.h citizen.h uniqueOrderedList/uniqueOrderedList.h candidate.h
mtmElections.o : mtmElections.c mtmElections.h city.h citizen.h uniqueOrderedList/uniqueOrderedList.h candidate.h
exampleMain.o: mtmElections.h exampleMain.c


clean:
	rm -f $(OBJS)
