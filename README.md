### git从入门到精通

#### 查看当前git安装成功没有

```
git --version
```

#### 配置信息

```sh
--global 参数，有了这个参数，表示你这台机器上所有的Git仓库都会使用这个配置
```

##### 为当前项目创建用户名

```sh
git config --global user.name 'deroy'
```

##### 为当前项目创建用户邮箱

```sh
git config --global user.email '2542639616@qq.com'
```

#### 创建版本库

> 通过命令 git init 把这个目录变成git可以管理的仓库

```sh
git init
```

创建成功后当前文件夹下会多一个.git的目录，这个目录是Git来跟踪管理版本的

所有的版本控制系统，只能跟踪文本文件的改动，比如txt文件，网页，所有程序的代码等，但是图片，视频这些二进制文件，虽能也能由版本控制系统管理，但没法跟踪文件的变化，只能把二进制文件每次改动串起来，也就是知道图片从1kb变成2kb，但是到底改了啥，版本控制也不知道

##### 查看文件有没有被纳入版本控制当中

创建一个html

```sh
touch index.html
```

```sh
git status index.html	#查看这个html的状态
```

##### 查看是否还有文件未提交

```sh
git status	#查看是否还有文件未提交
```

##### 查看文件修改内容

```sh
git diff index.html
```

##### 将文件添加到控制列表中

> 把文件添加到暂存区。

```sh
git add index.html
git add *.html#添加某一类文件
git add .#添加所有的文件
```

##### 将文件移除控制列表中

```sh
git rm --cached index.html
```

##### 提交队列当中的文件

> 把暂存区的所有内容提交到当前分支上

```sh
git commit
```

```sh
git commit -m '附加信息'	#提交的同时备注
```

#### 版本回退

##### 查看下历史记录

> git log命令显示从最近到最远的显示日志

```sh
git log	#查看详细信息
```

```sh
git log –pretty=oneline	#查看简略信息
```

##### 回退到以前的版本

```sh
git reset --hard HEAD^	#回到上一个版本
git reset --hard HEAD^^	#回到上上一个版本
```

```sh
git reset --hard HEAD~100	#回到100版本
```

回退到最新的版本

```sh
git reset --hard 版本号
```

###### 查看版本号

```sh
git reflog
```

#### 创建与合并分支

> 分支的好处就是 在分支里改代码不会影响主干的代码,切换到分支的话生成的代码,再切换到主干的时候那些修改的代码会消失,然后再切换到分支时又会出现,所以在分支测试好代码后再弄到主干里

```sh
git branch login	#创建分支
git checkout login  #切换到这个分支,
```

上面两行命令可以直接用下面的替换

```sh
git checkout -b login	#创建并切换分支
```

```sh
git branch #查看当前分支
```

```sh
git merge login		#主线合并分支的命令  主线里就有了分支的内容了
```

合并了分支之后可以删除分支

```sh
git branch -d login
```

#### 远程仓库

##### 查看项目对应仓库

```sh
git remote	#查看本项目有没有对应的仓库
```

```sh
$ git remote	#后面没有显示内容
```

如果有仓库

```sh
$ git remote	#后面显示仓库名称
origin
```

##### 删除仓库

```sh
git remote rm origin
```

##### 添加新的仓库

```sh
git remote add origin https://github.com/ADeRoy/C_Game_Dem.git	#这个仓库地址是我的，换成你们的
```

##### 推送到远程

```sh
git push -u origin master
```

推送成功后，可以立刻在github页面中看到远程库的内容已经和本地一模一样了

只要本地作了提交，就可以通过如下命令把本地master分支的最新修改推送到github上

```sh
git push origin master
```

现在你就拥有了真正的分布式版本库了

#### 下载github项目到当前文件夹

```sh
git clone https://github.com/lesliefish/Qt.git
```

### 