#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<iostream>
#include<ctype.h>
#include<string.h>
#include <stdlib.h>
#include<errno.h>
#include<unistd.h>
#include <hashlibpp.h>
#include<iomanip>
#include<conio.h>

using namespace std;
static inline void loadbar(unsigned long long x, unsigned long long n, unsigned int w = 50)
{
    if ( (x != n) && (x % (n/100) != 0) ) return;

     float ratio  =  (float)x/n;
    int   c      =  ratio * w;

    cout << setw(3) << (int)(ratio*100) << "% [";
    for (int x=0; x<c; x++) cout << "=";
    for (int x=c; x<w; x++) cout << " ";
    cout << "]\r" << flush;
}

static inline void loadbar( float z, unsigned long long x, unsigned long long n, unsigned int w = 50)
{
    if ( (x != n) && (x % (n/100) != 0) ) return;

    float ratio  =  (float)x/n;
    int   c      =  ratio * w;

    cout <<setw(10)<<z<<"%zero"<<setw(3) << (int)(ratio*100) << "% [";
    for (int x=0; x<c; x++) cout << "=";
    for (int x=c; x<w; x++) cout << " ";
    cout << "]\r" << flush;
}


int main(int argc, char* argv[]) {
    FILE *fd,*clusEntry,*fp,*zerocluster,*output,*output2;
    FILE *final;
	long result;
	int bytes_sector[2];
	unsigned char buf[512];
	unsigned long long bytes_sec_int;
	char *bytes;
	unsigned long long sector_cluster;
	int i = 0;
	int k = 11;
	//FILE *tempcluster;
	string logicalVolume, lv;


	//--------------------------------Extracting cluster content-----------------------------------------------
	if ( argc != 2 ) /* argc should be 2 for correct execution */
		    {
		        /* We print argv[0] assuming it is the program name */
		        printf( "usage: %s filename\n FileSystemFinder is exiting...\n", argv[0] );
		        return 0;

		    }
	//cout << "Enter logical volume to be scanned" << endl;
	//cin >> lv;
	lv=argv[1];
	int x;
	logicalVolume = "\\\\.\\";
	logicalVolume = logicalVolume + lv + ":";
	//cout << "entered volume" << logicalVolume;
//	fp = fopen("\\\\.\\I:", "rb");
	fp = fopen64(logicalVolume.c_str(), "rb");
	if (!fp) {
		printf("\nfp unsuccessful");
		cin>>x;
		return -2;
	}

	hashwrapper *myWrapper = new sha512wrapper();

	result = fread(buf, 512, 1, fp);
	// Bytes per sector calculation
	for (i = 0; i < 3; i = i + 2) {
		char buffer[2] = "";
		char s[2] = "0";

		itoa(buf[k], buffer, 16);

		if (strlen(buffer) < 2) {
			strcat(s, buffer);

			bytes_sector[i] = s[0];
			bytes_sector[i + 1] = s[1];
			//printf("%c,%c \n", bytes_sector[i],bytes_sector[i+1]);
			//printf("Hello");
		} else {
			bytes_sector[i] = buffer[0];
			bytes_sector[i + 1] = buffer[1];
			//printf("%c,%c \n", bytes_sector[i],bytes_sector[i+1]);
			//printf("%s \n",bytes_sector[i]);
		}
		//strcat(bytes_sec, bytes_sector[i]);
		//printf("%s \n",bytes_sec);
		k = k + 1;

	}
	//printf("%s \n",bytes_sec);
	char bytes_sec_big[5] = "";
	bytes_sec_big[0] = bytes_sector[2];
	bytes_sec_big[1] = bytes_sector[3];
	bytes_sec_big[2] = bytes_sector[0];
	bytes_sec_big[3] = bytes_sector[1];
	bytes_sec_big[8] = '\0';

	//printf("%s \n",bytes_sec_big);

	bytes_sec_int = strtol(bytes_sec_big, &bytes, 16);
	// Sector per cluster calculation
	sector_cluster = buf[13];
	printf("\n%d bytes per sector \n", bytes_sec_int);
	printf("%d sector per cluster \n", sector_cluster);

	fclose(fp);
    unsigned char c;
    unsigned long long clusternum=0,count=0,count_unalloc=0;

//--------------------------------------------------------processing Bitmap file----------------------------------

    fd = fopen64("$Bitmap", "rb");
    if(!fd){
    	cout<<"Cannot open bitmap"<<endl;
    	return -1;
    }

    clusEntry = fopen("clusterNumbers","wb");
    if(!clusEntry){
    	cout<<"cannot open clusterNumbers"<<endl;
    	return -1;
    }
   fseeko64(fd,0L,SEEK_END);
   unsigned long long sizeofbitmap = ftello64(fd);
   fseeko64(fd,0L,SEEK_SET);

   cout <<"----------------------Processing bitmap----------------------------------"<<endl;
   while(fread(&c,sizeof(c), 1, fd)){
	int clustnumPerByte=0;

	//   long result = fread(&c,sizeof(c), 1, fd);
//	 printf("<0x%x>\n", c);
/*	if (c==248){   //Look for ending character(0xF8)
		break;
	}*/
    c = (c * 0x0202020202ULL & 0x010884422010ULL) % 1023; // reverse c

		if ((c & 0x80)==0) {
			clusternum = (count * 8) + clustnumPerByte;
		//	cout<<clusternum<<endl;
			fwrite(&clusternum,sizeof(clusternum), 1, clusEntry);
			count_unalloc++;
		}
		clustnumPerByte++;
		if ((c & 0x40)==0) {
			clusternum = (count * 8) + clustnumPerByte;
		//	cout<<clusternum<<endl;
			fwrite(&clusternum,sizeof(clusternum), 1, clusEntry);
			count_unalloc++;
		}
		clustnumPerByte++;
		if ((c & 0x20)==0) {
			clusternum = (count * 8) + clustnumPerByte;
		//	cout<<clusternum<<endl;
			fwrite(&clusternum,sizeof(clusternum), 1, clusEntry);
			count_unalloc++;
		}
		clustnumPerByte++;
		if ((c & 0x10)==0) {
			clusternum = (count * 8) + clustnumPerByte;
		//	cout<<clusternum<<endl;
			fwrite(&clusternum,sizeof(clusternum), 1, clusEntry);
			count_unalloc++;
		}
		clustnumPerByte++;
		if ((c & 0x08)==0) {
			clusternum = (count * 8) + clustnumPerByte;
		//	cout<<clusternum<<endl;
			fwrite(&clusternum,sizeof(clusternum), 1, clusEntry);
			count_unalloc++;
		}
		clustnumPerByte++;
		if ((c & 0x04)==0) {
			clusternum = (count * 8) + clustnumPerByte;
		//	cout<<clusternum<<endl;
			fwrite(&clusternum,sizeof(clusternum), 1, clusEntry);
			count_unalloc++;
		}
		clustnumPerByte++;
		if ((c & 0x02)==0) {
			clusternum = (count * 8) + clustnumPerByte;
		//	cout<<clusternum<<endl;
			fwrite(&clusternum,sizeof(clusternum), 1, clusEntry);
			count_unalloc++;
		}
		clustnumPerByte++;
		if ((c & 0x01)==0) {
			clusternum = (count * 8) + clustnumPerByte;
		//	cout<<clusternum<<endl;
			fwrite(&clusternum,sizeof(clusternum), 1, clusEntry);
			count_unalloc++;
		}
	count++;
	loadbar(count,sizeofbitmap);
    if(feof(fd)){
    	break;
    }
   }
   //cout<<"Count"<<count<<endl;
   unsigned long long total_clus = (count)*8;
   cout <<endl<<"Total clusters"<<total_clus<<endl;
   cout<<"Unallocated clusters"<<count_unalloc<<endl;
   cout << " Percentage of unallocated clusters"<<std::fixed;
   cout<<((float)count_unalloc/(float)total_clus)*100;
   fclose(fd);
   fclose(clusEntry);

	//---------------------------------------------------------- Calculating sample hash for unallocated cluster
	//Size of 1 cluster in bytes
	const long clustSize = sector_cluster * bytes_sec_int;
	unsigned char clustContent[clustSize]; // Buffer to read one cluster
//	FILE *hashSample;
	string sample_hash;

/*	hashSample = fopen("Samplecluster.txt", "wb");
	if (!hashSample) {
		printf("\nCannot open Samplecluster.txt");
		exit(1);
	}*/

	for (i = 0; i < clustSize; i++) {
		clustContent[i] = 0;
	}
	string clustStr( clustContent, clustContent + sizeof clustContent / sizeof clustContent[0] );
/*	long bytes_written = fwrite(clustContent, clustSize, 1, hashSample);

	if (bytes_written != 1) {
		printf("\n bytes written %ld", bytes_written);
		fclose(hashSample);

		printf(
				"\n Some error occurred while writing Sample cluster to file.Exiting");
		return -2;
	} else {
		//printf("\n bytes written %ld", bytes_writen);
		sample_hash = myWrapper->getHashFromFile("Samplecluster.txt");
		myWrapper->getHashFromString("abc");
//		cout << endl << "Sample hash"<< sample_hash;

		fclose(hashSample);
		//check

	}*/
	sample_hash = myWrapper->getHashFromString(clustStr);
	cout<<endl<<"Sample hash"<<sample_hash<<endl;
	//final=fopen("Unallocate_Percentage.txt","w+");
	//************************************Sample hash calculation complete********************************************************************************************
	cout << "Total clusters to test" << count_unalloc << endl;
	clusEntry = fopen("clusterNumbers", "rb");
	zerocluster = fopen("zeroclusters.txt", "wb");
	fp = fopen64(logicalVolume.c_str(), "rb");
	if (!fp) {
		printf("\nfp unsuccessful");
		return -2;
	}
	if (!clusEntry) {
		printf("\nCannot open file clusterNumbers.txt for reading");
		exit(1);
	}
	if (!zerocluster) {
		printf("\nCannot open file zeroclusters.txt for writing");
		exit(1);
	}
	unsigned long long count_zero=0;
	unsigned long long clusterOffset;
	count=0;
	string temp_file = "output_"+lv+".txt";
cout<<"---------------------looking for zero clusters--------------------------------"<<endl<<endl;
	while (fread(&clusternum, sizeof(clusternum), 1, clusEntry)) {
 //   	cout <<endl<< "cluster:"<<clusternum << endl;
//		cout<<endl<<count++<<endl;
		count++;
		clusterOffset = clusternum * sector_cluster * bytes_sec_int;
		result = fseeko64(fp, clusterOffset, SEEK_SET);
		if (result != -1) {
			result = fread(clustContent, bytes_sec_int, sector_cluster, fp);
			string testStr( clustContent, clustContent + sizeof clustContent / sizeof clustContent[0] );
			string hash1 = myWrapper->getHashFromString(testStr);
			if (hash1.compare(sample_hash) == 0) {
				count_zero++;
				fwrite(&clusternum, sizeof(clusternum), 1, zerocluster);
	//					cout <<endl<<count_zero<<":zero count"<<endl;
			}

		}
		else{
			cout<<"Cannot fseek"<<endl;
			return -1;
		}
		//Writing temporary results to a file
				output2 = fopen(temp_file.c_str(), "w");
				if (!output2) {
					cout << endl << "Cannot write output2" << endl;
					cout << strerror(errno);
					getch();
					return 0;
				}
				fprintf(output2, "\n logical Volume: %s\n", logicalVolume.c_str());
				fprintf(output2, "NTFS File System (64 bit)\n");
				fprintf(output2, "Total Clusters:%llu\n", total_clus);
				fprintf(output2,"%d bytes per sector \n", bytes_sec_int);
				fprintf(output2,"%d sector per cluster \n", sector_cluster);
				fprintf(output2, "\nPercentage of unallocated clusters:%lf",
						((double) count_unalloc / (double) total_clus) * 100);
				fprintf(output2, "\nPercentage completion of task:%lf\n",
						((double) count / (double) count_unalloc) * 100);
				fprintf(output2, "\n Percentage of zero clusters: %lf\n",
						((double) count_zero / (double) total_clus) * 100);
				fprintf(output2,
						"********************************************************\n");
				fclose (output2);

		loadbar(((float)count_zero/total_clus)*100,count,count_unalloc);

		if (feof (clusEntry)) {
			break;
		}

	}

	cout <<endl<<"Percentage of clusters that are zero"<<((float)count_zero/total_clus)*100 <<endl;
	fclose(zerocluster);
	fclose(clusEntry);
	//Writing results in file
	output = fopen("output.txt","a+");
		if(!output){
			cout<<endl<<"Cannot write output"<<endl;
			getch();
			return 0;
		}
		fprintf(output,"\n logical Volume: %s\n",logicalVolume.c_str());
		fprintf(output,"NTFS File System (64 bit)\n");
		fprintf(output,"bytes per sector:%d",bytes_sec_int);
		fprintf(output,"\n sector per cluster:%d",sector_cluster);

		fprintf(output,"Total Clusters:%llu",total_clus);
		fprintf(output,"\nPercentage of unallocated clusters:%f",((float)count_unalloc/(float)total_clus)*100);
		fprintf(output,"\n Percentage of zero clusters: %f\n",((float)count_zero/(float)total_clus)*100);
		fprintf(output,"********************************************************\n");
		fclose(output);





	getch();

    return 0;
}



