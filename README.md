
# Chess Project

This is a chess game with an integrated AI.

### How to work

The main tool of our project will be the **GitHub Issues**. Everything that needs to be reported will be there (what needs to be done, what is being done, bugs encountered, help needed...).

**How to use Issues:**

I (DatUser) will be in charge of releasing these issues.
Whenever someone is assigned to an issue or assigns himself to it, he becomes responsible of it. He will be in charge of updating the state of this issue using the labels. More information can be provided to issues by commenting them.

***!!!DO NOT FORGET TO CHANGE LABEL TO DONE AND TO CLOSE THE ISSUE YOU DEALT WITH ONCE YOU FINISHED!!!***

***Label Code***

* *bug:* Something isn't working
* *done:* Task is finished
* *help needed:* Help is needed (assign people you need help from to the task)
* *in progress:* Task in progress
* *to do:* Task that needs to be done
* *question:* Not sure of what to do yet (assign everybody to these, someone may have a response)
* *optimization:* Thing that need further optimization, more a bonus than anything

***Debug flags***

-DCMAKE_BUILD_TYPE=RelWithDebInfo 

***To make this work and have sense, eveyone needs to check the issues regularly!***

***In order to merge branches***

We use rebase and do it that way:

git checkout *yourbranch*

git rebase master

git checkout master

git rebase *yourbranch*

git push origin master

git push origin -d *yourbranch* (this will delete your branch)
