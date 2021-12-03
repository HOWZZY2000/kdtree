## KD-tree
KD-tree is a binary search tree where data in each node is a K-Dimensional point in space.  
It has a construction time of O(nlogn) and range search of O(s + sqrt(n)) time complexity for second dimensions where s is the output size.

This project implemented a KD-tree for second dimensions including functions like build, range-search and delete along with other utility functions like print and reset. 
## Usage
I have made a make file for compiling. You can compile in command line:
~~~
$ make
$ ./a < sample.in 
~~~
To test the results. 

## Example
It accepts commands from stdin.   
For example:
```
ikd 3 3 4 2 2 1 1
```
Builds a 2D-tree with 3 points namely (3,  4), (2,  2) and (1,  1) 
<br/><br/>
```
skd 1 2 1 2
```
Searches for the point with x-axis in [1,  2) and y-axis in [1,  2)  
Outputing for the above tree:  
```
1 1
```
<br/>

```
pkd
```
Prints all nodes of the current kd-tree in the order they are visited during a preorder traversal.
