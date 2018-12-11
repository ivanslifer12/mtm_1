#TESTFL=exampleMain
#TESTFL=test_fb1_main
#TESTFL=mtmElections_test
#TESTFL=TestsFinal


CC=gcc
OBJS=uniqueOrderedList/uniqueOrderedList.o preference.o citizen.o city.o
EXEC=barslist
DEBUG=-g  #-g for debug
CFLAGS=-std=c99 -Wall -Werror -pedantic-errors $(DEBUG)

$(EXEC)  : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

#$(TESTFL).o: $(TESTFL).c mtm_elections.h  uniqueOrderedList/uniqueOrderedList.h



uniqueOrderedList.o: uniqueOrderedList/uniqueOrderedList.c  uniqueOrderedList/uniqueOrderedList.h
preference.o: preference.c preference.h
citizen.o: citizen.c citizen.h  preference.h uniqueOrderedList/uniqueOrderedList.h
city.o: city.c city.h citizen.h uniqueOrderedList/uniqueOrderedList.h


#uniqueOrderedList/uniqueOrderedList.o: uniqueOrderedList/uniqueOrderedList.h uniqueOrderedList/uniqueOrderedList.c



clean:
	rm -f $(OBJS)
