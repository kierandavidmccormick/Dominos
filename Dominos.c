#include "lib.c"

#define HANDSIZE 6				// 0 to 99
#define STARTVAL 11				// 1 to 13

/*
TODO:

*/

struct dom{
	int front;
	int back;
	int depth;
	int minReturnDepth;
	char name[6];
	bool flag;
	struct dom *parent;
	struct dom *child0;
	struct dom *child1;
	struct dom *child2;
	struct dom *child3;
};

void makeHand(struct dom hand[], int length);
void makeRandHand(struct dom hand[], int length);
void makeDom(struct dom *pos, int number, int front, int back);
void printDom(struct dom *dp);
void printAllDoms(struct dom hand[], int length);
bool checkHand(struct dom hand[], int length);
void flip(struct dom *dp);
void unflagAll(struct dom hand[], int length);
void markRoot(struct dom hand[], int length, int val);
void markChild(struct dom *op, struct dom *dp);
void freeDom(struct dom *dp, struct dom *op);
void reorderChildren(struct dom *dp);
bool c0in(struct dom *dp);
bool c1in(struct dom *dp);
bool c2in(struct dom *dp);
bool c3in(struct dom *dp);
bool depthIsOk(struct dom *op, struct dom *dp);
struct dom* getShortestTrain(struct dom hand[], int length);
bool isNode(struct dom *dp);
bool isValid(struct dom *dp);
void addDom(struct dom hand[], int length, struct dom *op);
bool checkChildren(struct dom hand[], int length);
bool checkChild(struct dom *dp, struct dom *ep);
void initialChildrenCheck(struct dom hand[], int length);
bool isTreeLinear(struct dom hand[], int length);
void refineTree(struct dom hand[], int length);
void printFinalString(struct dom hand[], int length, struct dom* dp);
void printFinalStrings(struct dom hand[], int length);
void printSecondaryStrings(struct dom hand[], int length);
void addAllDoubles(struct dom hand[], int length);
void addDouble(struct dom hand[], int length, struct dom *dp);
struct dom* getDouble(struct dom hand[], int length);
void insertDouble(struct dom *dp, struct dom *np);
bool fixDepth(struct dom hand[], int length);
void fixDepths(struct dom hand[], int length);
void iterate(struct dom hand[], int length);
int getNumDoubles(struct dom hand[], int length);

// null domino- domino does not exist
struct dom nullDom = {-1, -1, -1, -1, "null ", true, &nullDom, &nullDom, &nullDom, &nullDom, &nullDom};
// root domino- root of tree
struct dom root = {STARTVAL, STARTVAL, 0, 0, "root ", false, &root, &nullDom, &nullDom, &nullDom, &nullDom};
struct dom hand[HANDSIZE];

int main(){
	makeHand(hand, HANDSIZE);
	markRoot(hand, HANDSIZE, STARTVAL);
	initialChildrenCheck(hand, HANDSIZE);
	unflagAll(hand, HANDSIZE);
	refineTree(hand, HANDSIZE);
	iterate(hand, HANDSIZE);
	if (DEBUG){printAllDoms(hand, HANDSIZE);}
	printFinalStrings(hand, HANDSIZE);
	return 0;
}

void makeHand(struct dom hand[], int length){
	// make sure to change the handsize as well
	debug0("=> makeHandStart");
	struct dom *dp = &hand[0];
	int i = 0;
	makeDom(dp++, i++, 1, 1);
	makeDom(dp++, i++, 2, 2);
	makeDom(dp++, i++, 3, 3);
	makeDom(dp++, i++, 4, 4);
	makeDom(dp++, i++, 5, 5);
	makeDom(dp++, i++, 6, 6);
	if (!checkHand(hand, length)){
		printf("********************** hand cannot contain >4 of the same value\n");
	}
	if (i != HANDSIZE){
		printf("********************** # of doms must match HANDSIZE\n");
	}
	debug0("<= makeHandEnd");
}

void makeRandHand(struct dom hand[], int length){
	debug0("=> makeRandHandStart");
	struct dom *dp = &hand[0];
	for (int i = 0; i < length; i++, dp++){
		debug1("** makeRandHandIteration", i);
		makeDom(dp, i, gRand(13, 1), gRand(13, 1));
	}
	if (!checkHand(hand, length)){
		makeRandHand(hand, length);
	}
	debug0("<= makeRandHandEnd");
}

void makeDom(struct dom *pos, int number, int frontVal, int backVal){
	debug1("=> makeDomStart", number);
	pos->front = frontVal;
	pos->back = backVal;
	pos->depth = -1;
	pos->minReturnDepth = 0;
	strcat(pos->name, "dom");
	char temp[] = {'0', '0', '\0'};
	itos(temp, 3, number);
	strcat(pos->name, temp);
	pos->flag = false;
	pos->parent = &nullDom;
	pos->child0 = &nullDom;
	pos->child1 = &nullDom;
	pos->child2 = &nullDom;
	pos->child3 = &nullDom;
	debug0("<= makeDomEnd");
}

void printDom(struct dom *dp){
	debug0("=> printDomStart");
	printf("\n");
	printf("%s:\n", dp->name);
	printf("front: %d\n", dp->front);
	printf("back: %d\n", dp->back);
	printf("depth: %d\n", dp->depth);
	printf("minReturnDepth: %d\n", dp->minReturnDepth);
	if (dp->flag){
		printf("flag: true\n");
	} else {
		printf("flag: false\n");
	}
	if (dp->parent->name != dp->name){
		printf("parent: %s\n", dp->parent->name);
	} else {
		printf("parent: root\n");
	}
	printf("child0: %s\n", dp->child0->name);
	printf("child1: %s\n", dp->child1->name);
	printf("child2: %s\n", dp->child2->name);
	printf("child3: %s\n", dp->child3->name);
	printf("\n");
	debug0("<= printDomEnd");
}

void printAllDoms(struct dom hand[], int length){
	debug0("=> printAllDomsStart");
	printDom(&root);
	struct dom *dp = &hand[0];
	for (int i = 0; i < length; i++, dp++){
		debug1("** printAllDomsIteration", i);
		printDom(dp);
	}
	debug0("<= printAllDomsEnd");
}

bool checkHand(struct dom hand[], int length){
	debug0("=> checkHandStart");
	int valueCounts[14];
	struct dom *dp;
	for (int i = 0; i < 14; i++){
		valueCounts[i] = 0;
		debug1("** checkHandIteration0", i);
	}
	for (int i = 1; i < length; i++){
		dp = &hand[i];
		valueCounts[dp->front]++;
		valueCounts[dp->back]++;
		debug3("** checkHandIteration1", i, valueCounts[dp->front], valueCounts[dp->back]);
	}
	for (int i = 1; i <14; i++){
		debug2("** checkHandIteration2", i, valueCounts[i]);
		if (valueCounts[i] > 4){
			debug0("<= checkHandEnd");
			return false;
		}
	}
	if (DEBUG){printArray(valueCounts, 14);}
	debug0("<= checkHandEnd");
	return true;
}

void flip(struct dom *dp){
	debug2("=> flipStart", dp->front, dp->back);
	int temp = dp->front;
	dp->front = dp->back;
	dp->back = temp;
	debug0("<= flipEnd");
}

void unflagAll(struct dom hand[], int length){
	debug0("=> unflagAllStart");
	struct dom *dp;
	for(int i = 0; i < length; i++){
		dp = &hand[i];
		dp->flag = false;
	}
	root.flag = false;
	debug0("<= unflagAllEnd");
}

void markRoot(struct dom hand[], int length, int val){
	debug1("=> markRootStart", val);
	struct dom *dp;
	for(int i = 0; i < length; i++){
		dp = &hand[i];
		if(dp->front == val){
			markChild(&root, dp);
		} else if (dp->back == val){
			markChild(&root, dp);
			flip(dp);
		}
	}
	debug0("<= markRootEnd");
}

void markChild(struct dom *op, struct dom *dp){
// mark dp as a child of op
	debug0("=> markChildStart");
	if (op->child0 == &nullDom){
		op->child0 = dp;
		dp->parent = op;
		dp->depth = op->depth + 1;
		dp->flag = true;
		if (DEBUG){printf("** struct %s set as child0 of struct %s\n", dp->name, op->name);}
	} else if (op->child1 == &nullDom){
		op->child1 = dp;
		dp->parent = op;
		dp->depth = op->depth + 1;
		dp->flag = true;
		if (DEBUG){printf("** struct %s set as child1 of struct %s\n", dp->name, op->name);}
	} else if (op->child2 == &nullDom){
		op->child2 = dp;
		dp->parent = op;
		dp->depth = op->depth + 1;
		dp->flag = true;
		if (DEBUG){printf("** struct %s set as child2 of struct %s\n", dp->name, op->name);}
	} else if (op->child3 == &nullDom){
		op->child3 = dp;
		dp->parent = op;
		dp->depth = op->depth + 1;
		dp->flag = true;
		if (DEBUG){printf("** struct %s set as child3 of struct %s\n", dp->name, op->name);}
	} else {
		printf("************ %s has too many children\n", op->name);
	}
	debug0("<= markChildEnd");
}

void freeDom(struct dom *dp, struct dom *op){
	if (DEBUG){printf("=> freeDomStart, dp: %s, op: %s\n", dp->name, op->name);}
	dp->parent = &nullDom;
	dp->minReturnDepth = dp->depth + 1;
	dp->depth = -1;
	if (op->child3 == dp){
		op->child3 = &nullDom;
		if (DEBUG){printf("** removing dom: %s as child3 of %s\n", dp->name, op->name);}
	} else if (op->child2 == dp){
		op->child2 = &nullDom;
		if (DEBUG){printf("** removing dom: %s as child2 of %s\n", dp->name, op->name);}
	} else if (op->child1 == dp){
		op->child1 = &nullDom;
		if (DEBUG){printf("** removing dom: %s as child1 of %s\n", dp->name, op->name);}
	} else if (op->child0 == dp){
		op->child0 = &nullDom;
		if (DEBUG){printf("** removing dom: %s as child0 of %s\n", dp->name, op->name);}
	} else {
		printf("** %s not a child of %s\n", dp->name, op->name);
	}
	reorderChildren(op);
	debug0("<= freeDomEnd");
}

void reorderChildren(struct dom *dp){
	if (DEBUG){printf("=> reorderChildrenStart: %s\n", dp->name);}
	if ((!c3in(dp)) && c2in(dp)){
		if (DEBUG){printf("** reordering child3 to child2\n");}
		dp->child2 = dp->child3;
		dp->child3 = &nullDom;
		reorderChildren(dp);
	} else if ((!c2in(dp)) && c1in(dp)){
		if (DEBUG){printf("** reordering child2 to child1\n");}
		dp->child1 = dp->child2;
		dp->child2 = &nullDom;
		reorderChildren(dp);
	} else if ((!c1in(dp)) && c0in(dp)){
		if (DEBUG){printf("** reordering child1 to child0\n");}
		dp->child0 = dp->child1;
		dp->child1 = &nullDom;
		reorderChildren(dp);
	}
	debug0("<= reorderChildrenEnd");
}

bool c0in(struct dom *dp){
	if (dp->child0->depth < 0){
		debug0("** c0it");
		return true;
	}
	debug0("** c0if");
	return false;
}

bool c1in(struct dom *dp){
	if (dp->child1->depth < 0){
		debug0("** c1it");
		return true;
	}
	debug0("** c1if");
	return false;
}

bool c2in(struct dom *dp){
	if (dp->child2->depth < 0){
		debug0("** c2it");
		return true;
	}
	debug0("** c2if");
	return false;
}

bool c3in(struct dom *dp){
	if (dp->child3->depth < 0){
		debug0("** c3it");
		return true;
	}
	debug0("** c3if");
	return false;
}

struct dom* getShortestTrain(struct dom hand[], int length){
	debug0("=> getShortestTrainStart");
	struct dom *dp;
	struct dom *np;
	int shortest = length + 1;
	for (int i = 0; i < length; i++){
		dp = &hand[i];
		if (DEBUG){printf("** getShortestTrain dp: %s\n", dp->name);}
		if (isValid(dp) && (dp != &nullDom) && (dp->child0 == &nullDom)){
			if (dp->depth < shortest){
				shortest = dp->depth;
				np = dp;
			}
		}
	}
	if (DEBUG){printf("<= getShortestTrainEnd, np: %s\n", np->name);}
	return np;
}

bool isNode(struct dom *dp){
	if (DEBUG){printf("=> isNodeStart %s\n", dp->name);}
	if (dp->parent == &nullDom){
		if (DEBUG){printf("=> isNodeEnd %s false\n", dp->name);}
		return false;
	} else if (dp->parent == &root){
		if (DEBUG){printf("=> isNodeEnd %s true\n", dp->name);}
		return true;
	}
	if (dp->child1 != &nullDom){
		debug0("<= isNodeEnd true");
		return true;
	}
	debug0("<= isNodeEnd false");
	return false;
}

bool isValid(struct dom *dp){
	if (DEBUG){printf("=> isValidStart %s\n", dp->name);}
	struct dom *np = dp;
	if (dp->parent == &nullDom){
		if (DEBUG){printf("=> isValidEnd %s false\n", np->name);}
		return false;
	} else if (dp->parent == &root){
		if (DEBUG){printf("=> isValidEnd %s true\n", np->name);}
		return true;
	}
	while(!isNode(dp)){
		dp = dp->parent;
	}
	if (dp->front == dp->back){
		if (dp->child2 != &nullDom){
			if (DEBUG){printf("=> isValidEnd %s true\n", np->name);}
			return true;
		}
		if (DEBUG){printf("=> isValidEnd %s false\n", np->name);}
		return false;
	} else {
		if (dp->child1 != &nullDom){
			if (DEBUG){printf("=> isValidEnd %s true\n", np->name);}
			return true;
		}
		if (DEBUG){printf("=> isValidEnd %s false\n", np->name);}
		return false;
	}
}

struct dom* freeShortestDom(struct dom hand[], int length){
	debug0("=> freeShortestDomStart");
	struct dom *dp = getShortestTrain(hand, length);
	freeDom(dp, dp->parent);
	debug0("<= freeShortestDomEnd");
	return dp;
}

void addDom(struct dom hand[], int length, struct dom *op){
	if (DEBUG){printf("=> addDomStart, %s, minReturnDepth: %d\n", op->name, op->minReturnDepth);}
	struct dom *dp;
	for (int i = 0; i < length; i++){
		dp = &hand[i];
		if (DEBUG){printf("** dp: %s, dp->depth: %d\n", dp->name, dp->depth);}
		if (op->front == dp->back && op != dp && op->minReturnDepth <= dp->depth + 1){
			markChild(dp, op);
			checkChildren(hand, length);
		} else if (op->back == dp->back && op != dp && op->minReturnDepth <= dp->depth + 1){
			flip(op);
			markChild(dp, op);
			checkChildren(hand, length);
		}
	}
	debug0("<= addDomEnd");
}

bool checkChildren(struct dom hand[], int length){
	debug1("=> checkChildrenStart", length);
	bool addedChild = false;
	struct dom *dp;
	struct dom *ep;
	for (int i = 0; i < length; i++){
		dp = &hand[i];
		if (DEBUG){printf("**** checkChildrenIteration dp: %s i: %d\n", dp->name, i);}
		for (int j = 0; j < length; j++){
			ep = &hand[j];
			if (DEBUG){printf("** checkChildrenIteration ep: %s j: %d\n", ep->name, j);}
			if (ep->parent->depth == -1 && dp != ep && dp->parent->depth != -1){
				if (checkChild(dp, ep)){
					addedChild = true;
				}
			}
		}
	}
	return addedChild;
	debug0("<= checkChildrenEnd");
}

bool checkChild(struct dom *dp, struct dom *ep){
	if (DEBUG){printf("** checking if %s can be a child of %s\n",ep->name, dp->name );}
	if (dp->back == ep->front){
		markChild(dp, ep);
		debug0("<= checkChildEnd");
		return true;
	} else if (dp->back == ep->back){
		flip(ep);
		markChild(dp, ep);
		debug0("<= checkChildEnd");
		return true;
	}
	debug0("<= checkChildEnd");
	return false;
}

void initialChildrenCheck(struct dom hand[], int length){
	debug0("=> initialChildrenCheckStart");
	if (checkChildren(hand, length)){
		initialChildrenCheck(hand, length);
	}
	debug0("<= initialChildrenCheckEnd");
}

bool isTreeLinear(struct dom hand[], int length){
	debug0("=> isTreeLinearStart");
	struct dom *dp;
	if (root.child1->depth > 0){
		if (DEBUG){printf("** root is not linear\n");}
		debug0("<= isTreeLinearEnd (NO)");
		return false;
	} else {
		if (DEBUG){printf("** root is linear\n");}
	}
	for (int i = 0; i < length; i++){
		dp = &hand[i];
		if (dp->child2->depth > 0){
			if (DEBUG){printf("** %s is not linear, i: %d\n", dp->name, i);}
			debug0("<= isTreeLinearEnd (NO)");
			return false;
		} else if (dp->child1->depth > 0 && dp->front != dp->back){
			if (DEBUG){printf("** %s is not linear, i: %d\n", dp->name, i);}
			debug0("<= isTreeLinearEnd (NO)");
			return false;
		} else {
			if (DEBUG){printf("** %s is linear, i: %d\n", dp->name, i);}
		}
	}
	debug0("<= isTreeLinearEnd (YES)");
	return true;
}

void refineTree(struct dom hand[], int length){
	debug0("=> refineTreeStart");
	int temp = 0;
	while (!isTreeLinear(hand, length) && temp < 500){
		addDom(hand, length, freeShortestDom(hand, length));
		temp++;
		debug1("** refineTreeIteration", temp);
	}
	debug0("<= refineTreeEnd");
}

void printFinalString(struct dom hand[], int length, struct dom* dp){
	debug0("=> printFinalStringStart");
	if (DEBUG){printf("** printing dom: %s\n", dp->name);}
	printf("%d/%d", dp->front, dp->back);
	if (dp->child1 != &nullDom){
		printf("* ");
	} else {
		printf(" ");
	}
	if (dp->child0->depth > 0){
		printFinalString(hand, length, dp->child0);
	} else {
		if (DEBUG){printf("** dp->child0->depth !> 0 %s\n", dp->child0->name);}
	}
	debug0("<= printFinalStringEnd");
	if (dp->depth == 0){
		printf("\n");
	}
}

void printFinalStrings(struct dom hand[], int length){
	debug0("=> printFinalStringsStart");
	printf("A * next to a domino indicates the presence of a secondary string attached to that domino\n\n");
	printf("Main string:\n");
	printFinalString(hand, length, &root);
	printf("\nSecondary strings:\n");
	printSecondaryStrings(hand, length);
	debug0("<= printFinalStringsEnd");
}

void printSecondaryStrings(struct dom hand[], int length){
	debug0("=> printSecondaryStringsStart");
	struct dom *dp;
	if (root.child1 != &nullDom){
		printFinalString(hand, length, root.child1);
	}
	for (int i = 0; i < length; i++){
		dp = &hand[i];
		if (dp->child1 != &nullDom){
			printFinalString(hand, length, dp->child1);
		}
	}
	debug0("<= printSecondaryStringsEnd");
	printf("\n");
}

void addAllDoubles(struct dom hand[], int length){
	debug0("=> addAllDoublesStart");
	struct dom *dp = getDouble(hand, length);
	while (dp != &nullDom){
		addDouble(hand, length, dp);
		dp = getDouble(hand, length);
	}
	fixDepths(hand, length);
	unflagAll(hand, length);
	debug0("<= addAllDoublesEnd");
}

void addDouble(struct dom hand[], int length, struct dom *dp){
	if (DEBUG){printf("=> addDoubleStart %s\n", dp->name);}
	struct dom *sp;
	for (int i = 0; i < length; i++){
		sp = &hand[i];
		if (dp->front == sp->front && dp != sp){
			insertDouble(dp, sp);
		}
	}
	debug0("<= addDoubleEnd");
}

struct dom* getDouble(struct dom hand[], int length){
	debug0("=> getDoubleStart");
	struct dom *dp;
	for (int i = 0; i < length; i++){
		dp = &hand[i];
		if (dp->front == dp->back && dp->parent->depth == -1 && dp->flag == false){
			if (DEBUG){printf("** returned %s\n", dp->name);}
			dp->flag = true;
			debug0("<= getDoubleEnd");
			return dp;
		}
	}
	debug0("** returned nulldom");
	debug0("<= getDoubleEnd");
	return &nullDom;
}

void insertDouble(struct dom *dp, struct dom *cp){
	debug0("=> insertDoubleStart");
	struct dom *pp = cp->parent;
	if (DEBUG){printf("** inserting %s between parent %s and child %s\n", dp->name, pp->name, cp->name);}
	dp->child0 = cp;
	cp->parent = dp;
	dp->parent = pp;
	if (pp->child0 == cp){
		pp->child0 = dp;
	} else if (pp->child1 == cp){
		pp->child1 = dp;
	} else if (pp->child2 == cp){
		pp->child2 = dp;
	} else if (pp->child3 == cp){
		pp->child3 = dp;
	} else {
		printf("****** could not insert dom %s as child of %s\n", dp->name, pp->name);
	}
	debug0("<= insertDoubleEnd");
}

bool fixDepth(struct dom hand[], int length){
	debug0("=> fixDepthStart");
	struct dom *dp;
	for (int i = 0; i < length; i++){
		dp = &hand[i];
		if (dp->parent->depth > -1 && dp->depth != dp->parent->depth + 1){
			if (DEBUG){printf("** %s->depth was %d, is %d\n", dp->name, dp->depth, dp->parent->depth + 1);}
			dp->depth = dp->parent->depth + 1;
			debug0("<= fixDepthEnd");
			return true;
		}
	}
	debug0("<= fixDepthEnd");
	return false;
}

void fixDepths(struct dom hand[], int length){
	debug0("=> fixDepthsStart");
	while(fixDepth(hand, length)){
		;
	}
	debug0("<= fixDepthsEnd");
}

void iterate(struct dom hand[], int length){
	debug0("=> iterateStart");
	for (int i = 0; i > getNumDoubles(hand, length); i++){
		refineTree(hand, HANDSIZE);
		addAllDoubles(hand, HANDSIZE);
	}
	debug0("<= iterateEnd");
}

int getNumDoubles(struct dom hand[], int length){
	debug0("=> getNumDoublesStart");
	int doubles = 0;
	struct dom *dp = &hand[0];
	for (int i = 0; i < length; i++){
		dp = &hand[i];
		if (dp->front == dp->back){
			doubles++;
		}
	}
	debug1("<= getNumDoublesEnd", doubles);
	return doubles;
}






















