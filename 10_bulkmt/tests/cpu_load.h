#include "stdlib.h"
#include "stdio.h"
#include "string.h"

/// взято отсюда https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

namespace CPU {
static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

void init(){
   FILE* file = fopen("/proc/stat", "r");
   fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
      &lastTotalSys, &lastTotalIdle);
   fclose(file);
}

double getCurrentValue(){
   double percent;
   FILE* file;
   unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

   file = fopen("/proc/stat", "r");
   fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
      &totalSys, &totalIdle);
   fclose(file);

   if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
      totalSys < lastTotalSys || totalIdle < lastTotalIdle){
      //Overflow detection. Just skip this value.
      percent = -1.0;
   }
   else{
      total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
         (totalSys - lastTotalSys);
      percent = total;
      total += (totalIdle - lastTotalIdle);
      percent /= total;
      percent *= 100;
   }

   lastTotalUser = totalUser;
   lastTotalUserLow = totalUserLow;
   lastTotalSys = totalSys;
   lastTotalIdle = totalIdle;

   return percent;
}

} // namespace CPU {