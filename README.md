Intelligent-Imaging
===================

Intelligent Imaging aims at detecting the percentage of unallocated unused clusters by reading the file system of Hard Disk or any other attached media at Byte level

There are six subfolders in the Project Folder. Each subfolder is a Eclipse Project Folder containing code written to accompalish particular task.

FAT32Imaging: This code aims to find the percentage of unallocated unused clusters in FAT32 File System. This code will work only for FAT32 File System.

FileSystemFinder: This code will identify the type of File System whether NTFS or FAT32 of the Hard Disk or attached media.

FilesystemForensics_win_64bit_exe: This folder contains the exe files and Batch files that can executed with a simple double click.
	Main File to be executed in this folder is "FileSystemForensics" Batch File. This file will call other executable files itself. User will have to only run this batch file and follow the prompt. At the end of the process user will get to see the percentage of unallocated clusters and the result for each logical volume/ Partition will also be saved in a .txt file for reference in future.

hashC: This folder contains the Library that was used to implement Hash Algorithm

NTFImaging: This code aims to find the percentage of unallocated unused clusters in NTFS File System. This code will work only for NTFS File System.
PartitionFinder: This code will identify the number of logical volumes/ Partitions on the Hard Disk eg - Drive C:, D:, E:, F: etc


