int pageSearch(int page) {
	int result = -1;
	if(!pageTable -> first != NULL) {
		Node *temp = pageTable -> first;
		while(temp != NULL) {
			if(temp -> page == page){
				result = temp -> page;
				break;
			}
			temp = temp -> next;
		}
	} else {
		printf("\n[ERROR] - PageIndexOutOfBounds");
	}
	return result;
}