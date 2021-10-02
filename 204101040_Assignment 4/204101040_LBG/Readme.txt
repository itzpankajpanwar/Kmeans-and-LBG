                                     LBG Algorithm Implementation

Name : Pankaj Panwar
Roll No : 204101040
MTech CSE 1st year

How to Run Project:- Open project in visual studio 2010 and run main.cpp file located in source files
		To view the source code go to View -> Solution Explorer -> main.cpp


Implementation Approach :

step 1: intially read data from universe 
step 2: store it into a vector of vector 
step 3 :here each vector contains 12 values 
step 4: Now make a initial codebook of size 1 (i.e it contains 1 vectors and this vector is centroid of universe)
step 5: split current codebook ( i.e. for each vector make 2 vetors uisng spiltting parameter )
step 6: Update current codebook using kmeans algorithm 
	i.e : for every vector in universe do the following 
	      calculate its distance from all others vectors in codebook and find minimum distane (here tokura distance is used)
	      Now add that vetor to corresponding vector in temporary codebook
              store minimum distane to distance vector 
	now after traversing all vectors in universe 
	we have a temporarybcodebook and a distance vector 
	take average of distanc vector  this is our distortion for this iteration
	update codebook using temporary codebook
	(6.1) check if distortion is less than thresold value 
	    if(yes)
           	 then print final codebook and terminate code
	    else 
	   	 goto step 6
step 7: if codebook size is 8 then terminate otherwise go to step 5