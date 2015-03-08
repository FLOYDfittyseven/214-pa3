struct record {
	
	char * filename;
	int tCount;
};

/* recordCompare compares two records. It compares them first by
 * count, then by filename if the counts are equal. It returns
 * -1 if arg2 goes before arg1 in the sorted-list, 1 if arg1 goes
 * before arg2 in the sorted-list, and 0 if they are identical.
 */
int recordCompare (struct record *arg1, struct record *arg2);

