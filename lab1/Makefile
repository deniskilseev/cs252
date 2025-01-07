all:: git

HW=1

git:
		git checkout master >> .local.git.out || echo
		git add . >> .local.git.out || echo
		git commit -a -m "Commit lab $(HW)" >> .local.git.out || echo
		git push origin master

submit:
		git checkout master >> .local.git.out || echo
		git add .  >> .local.git.out || echo
		git commit --allow-empty -a -m "Submission for lab $(HW)" >> .local.git.out || echo
		git push origin master
