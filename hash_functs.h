int compareFilesWithCount(void *p1, void *p2)
{
	FileWithCount s1 = p1;
	FileWithCount s2 = p2;

	return strcmp(s1->filename, s2->filename);
}



