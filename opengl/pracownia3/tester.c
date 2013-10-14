// Parsing PGM file from tifftopng geotiff
// (geotiff from SRTM:http://srtm.csi.cgiar.org/SELECTION/listImages.asp)
// 6000x6000 data for 15-20 50-55 degrees
// FILE NAME as argv[1] no error checking !!!!!!!!!!!!!!
// Quick test code by A.Lukaszewski  2008
#include <stdio.h>

char buf[1024];
short int nums[6000*6000];
static int hist[66000];

int main(int argc, char *argv[]){
   int i,j;
   FILE *in=fopen(argv[1], "rb");

   fgets(buf, 1000,  in);
   printf("%s", buf);
   fgets(buf, 1000,  in);
   printf("%s", buf);
   fgets(buf, 1000,  in);
   printf("%s", buf);
 
   fread(nums, 2, 6000*6000, in);
   fclose(in);

   char *ptr = (char*)nums;
   for (i=0; i<6000*6000; i++) {
      char tmp   = ptr[2*i];
      ptr[2*i]   = ptr[2*i+1];
      ptr[2*i+1] = tmp;
      printf("%d\n", nums[i]);
   }

   for (j=5990; j<6000; j++) {
      for (i=0; i<10; i++)
         printf("%4d ", nums[i+j*6000]);
      printf("\n");
   }


   for (j=0; j<6000; j++) 
      for (i=0; i<6000; i++) {
         int ht  = nums[i+j*6000];
         if(ht > 0){
            hist[ht]++;
        }
         // if (ht<0) printf("negative: %d\n", ht);
      }
   for (j=0; j<1700; j++) 
      printf("H[%5d] = %5d\n", j, hist[j]);

   return 0;
}  

