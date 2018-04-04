## Mini Twitter Interface

- **Name**: Tianyu Hu

* To make all my files work, put them and the test files in the same folder
* To compile, use `qmake`, `make`
* To run my program, use `./<executable name> <input file>`, here the `<executable name>` is `minitwitter`
* As to the trending feature, the hashtags that have different occurence numbers are guaranteed to be ordered correctly. But if some hashtags have same occurence numbers, their order relative to each other can alternate after refreshing. This is because I use pop() five times from my heap to get the top five hashtags and then push() them back into the heap. So the hashtags that have same occurence numbers as others are not guarantted to trickle up to their original location in the heap. Professor Redekopp says this is acceptable. 
* The following features are the same as in my homework 6:
	1. I uses a combobox at top-left of the GUI to choose user, and another combobox under it to choose the displays of main feed, mentions, followers, and the followings. The content will alternately display on the main display widget based on choice
	2. The functionality of saving data is triggered by the `Quit` button in my main GUI 
	3. The functionality of following a user is implemented with the `Find User` button. Click the `Find User` button, there will be a new window showing up. In the new window, search for the user you want to follow, then click on the desired result, then press the follow button at the bottom to follow the user
	4. Other features are same as requried in the homework requirements