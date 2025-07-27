---
created_at: 2025/07/25 23:22
last_modify_at: 2025/07/27 09:59
---

# 深度学习的数学

## chap2 神经网络的数学基础

### 函数

#### 定义

有两个变量$x$和$y$，如果对每个$x$都有唯一确定的$y$与它对应，则称$y$是$x$的函数，用$y=f(x)$表示，此时称$x$为自变量，$y$为因变量。

#### 一次函数

$$
y = ax + b \quad \text{(a、b为常数，}a\ne 0\text{)}\quad \quad(1)
$$

上面式子中$a$称为斜率，$b$称为截距。上面式子的函数图像如下：

![](https://static.cyub.vip/images/202507/epub_27337415_155.jpg)

当两个变量$x$、$y$满足式(1)的关系时候，称变量$y$和变量$x$是一次函数关系。

对于多个自变量，比如，有两个变量$x_1$,$x_2$,当它们满足下式的关系时，称$y$和$x_1$、$x_2$是一次函数关系：

$$
y = ax_1 + bx_2 + c \quad{(a、b、c为常数，a\ne 0, b\ne 0)}
$$

#### 神经单元的加权输入与一次函数

在神经网络中，神经单元的加权输入可以表示为一次函数关系。比如下面是神经单元有三个来自下层的输入，其加权输入$z$的表达式：

$$
z = w_1x_1 + w_2x_2 + w_3x_3 + b
$$

如果把作为参数的权重$w_1$、$w_2$、$w_3$与偏置$b$看作常数，那么加权输入$z$和$x_1$、$x_2$、$x_3$是一次函数关系。另外，在神经单元的输入$x_1$、$x_2$、$x_3$作为数据值确定了情况下，加权输入$z$和权重$w_1$、$w_2$、$w_3$以及偏置$b$是一次函数关系。

#### 二次函数

二次函数由下式表示：

$$
y = ax^2 + bx + c \quad{(a、b、c为常数，a\ne 0)}
$$

#### 单位阶跃函数

神经网络的原型函数是用单位阶跃函数作为激活函数的。它的图像和式子如下；

$$
u(x) =\left \{
\begin{array}{l} % l 靠左、c 居中、r 靠右
0 ( x\gt 0)\\\\
1 (x \ge 0)
\end{array}
\right.
$$

![](https://static.cyub.vip/images/202507/epub_27337415_224.jpg)

由于单位阶跃函数在原点处不可导，它不能成为主要的激活函数。

#### 指数函数

具有以下形状的函数称为指数函数：

$$
y = a^x\quad{(a为正的常数，a\ne 1)}
$$

常数$a$称为指数函数的底数。纳皮尔数$e$是一个特别重要的底数，其近似值如下：

$$
e = 2.71828\cdots
$$

##### Sigmoid函数

Sigmoid函数时神经网络中具有代表性的激活函数。

$$
\sigma(x) = \frac{1}{1+e^{-x}} = \frac{1}{1 + exp(-x)}
$$

上面公式中exp是exponential function(指数函数)的简略记法，$exp(x)$表示指数函数$e^x$。

![](https://static.cyub.vip/images/202507/epub_27337415_237.jpg)

### 数列

#### 定义

**数列是数的序列**。数列中的每一个数称为**项**。排在第一位的项称为**首项**，排在第二位的项称为第$2$项，排在第$3$的项称为第三项，以此类推，排在第$n$位的项称为第$n$项。

具有有限项的数列称为有穷数列，在有穷数列中，数列的最后一项称为**末项**。

#### 数列的通项公式

数列中排在第$n$位的数，通常用$a_n$表示，整个数列可以用符号${a_n}$来表示。

将数列的第$n$项用一个关于$n$的式子表示出来，这个式子就称为该数列的通项公式。

#### 数列与递推关系式

除了通项公式，数列还可以使用相邻项的关系式来表示，这种表示法称为数列的递归定义。

一般地，如果已知首项$a_1$以及相邻两项$a_n$、$a_{n+1}$的关系式，就可以确定这个数列，这个关系式称为 **递推关系式**。

已知首项$a_1 = 1$，以及关系式$a_{n+1} = a_n + 2$,可以确定一下数列，这个关系式就是数列的递推关系式。

$$
\begin{aligned}
a_1 &= 1\\
a_2 &= a_{1+1} = a_1 + 2 = 1+2 = 3\\
a_3 &= a_{2+1} = a_2 + 2 = 3 + 2 = 5\\
a_4 &= a_{3+1} = a_3 + 2 = 5 + 2 = 7\\
\cdots
\end{aligned}
$$

![](https://static.cyub.vip/images/202507/epub_27337415_276.jpg)

#### 联立递推关系式

将多个数列的递推关系式联合起来组成一组，称为 **联立递推关系式**。在神经网络的世界中，所以神经单元的输入和输出在数学上都可以认为是用联立递推式联系起来的。

### Σ符号

$\sum$ 符号用于表示数列的总和。

> [!warning] 注意
$\sum$为希腊字母，对应拉丁字母$S$，即Sum的首字母。 在希腊本土或纯希腊字母表中，$\sum$ 和 $\sigma$ 两者均读 "sigma"。但数学语境下通常把求和符号 ∑ 读作 "sigma（求和）"，而 σ 读作 "sigma（变量）"。

对于数列${a_n}$，$\sum$符号的定义式如下所示：

$$
\sum_{k=1}^{n}a_k = a_1 + a_2 + a_3 + ... + a_{n-1} + a_n
$$

上面式子中，字母$k$并具有实质的含义，$k$仅用于表明关于它求和，这个字母并非必须$k$，在数学上通常使用$i$、$j$、$k$、$l$、$m$、$n$。

#### Σ符号的性质

**$\sum$符号具有线性性质**。与导数公式“和导数为导数的和”，“常数倍的导数为导数的常数倍”类似，“和的$\sum$为$\sum$的和”，“常数倍的$\sum$为$\sum$的常数倍。

$$
\sum_{k=1}^n(a_k + b_k) = \sum_{k=1}^na_k + \sum_{k=1}^nb_k \quad(1)
$$

$$
\sum_{k=1}^nca_k = c\sum_{k=1}^na_k\quad{（c为常数）}
$$

##### 证明

根据$\sum$符号的定义，有：

$$
\begin{aligned}
\sum_{k=1}^n(a_k + b_k) &= (a_1 + b_1) +(a_2 + b_2) + \cdots + (a_n + b_n)\\
&= (a_1 + a_2 + \cdots + a_n) + (b_1 + b_2 + \cdots + b_n)\\
&= \sum_{k=1}^na_k + \sum_{k=1}^nb_k
\end{aligned}
$$

$$
\begin{aligned}
\sum_{k=1}^nca_k &= ca_1 + ca_2 + \cdots + ca_n \\
&= c(a_1 + a_2 + \cdots + a_n)\\
&= c\sum_{k=1}^na_k
\end{aligned}
$$

### 向量

#### 有向线段与向量

有两个点$A$、$B$，我们考虑从$A$指向$B$的线段，这条具有方向的线段$AB$叫做有向线段，其中$A$为起点，$B$为终点。

![](https://static.cyub.vip/images/202507/epub_27337415_342.jpg)

有向线段$AB$具有以下属性：起点$A$的位置，指向$B$的方向，以及$AB$的长度（即大小）。在三个属性中，把方向和大小抽象出来，这样的量叫做向量。

> 向量是具有方向和大小的量

有向线段$AB$所大表的向量用$\overrightarrow{AB}$表示，也可以用带箭头的单个字母$\mathbf{\overrightarrow{a}}$或者不带箭头的黑斜体字母$\mathbf{a}$表示。

![](https://static.cyub.vip/images/202507/epub_27337415_351.jpg)

#### 向量的坐标表示

把箭头的起点放在原点，用箭头终点的坐标表示向量，这叫做向量的坐标表示。

#### 向量的大小

从直观上来讲，表示向量的箭头的长度称为这个向量的大小。向量$\mathbf{a}$的大小用$|\mathbf{a}|$表示。

#### 向量的内积

两个向量$\mathbf{a}$、$\mathbf{b}$的内积$\mathbf{a}\cdot\mathbf{b}$的定义如下：

$$
\mathbf{a}\cdot\mathbf{b} = |\mathbf{a}||\mathbf{b}|\cos\theta\quad{（\theta为\mathbf{a},\mathbf{b}的夹角）}
$$

##### 柯西-施瓦茨不等式

$$
-|\mathbf{a}||\mathbf{b}| \le \mathbf{a}\cdot \mathbf{b} \le |\mathbf{a}||\mathbf{b}|
$$

证明如下：

根据余弦函数的性质，对任意的$\theta$ ,有$-1 \le \cos\theta \le 1$,两边同时乘以$|\mathbf{a}||\mathbf{b}|$，有：

$$
-|\mathbf{a}||\mathbf{b}| \le \mathbf{a}\cdot \mathbf{b} \le |\mathbf{a}||\mathbf{b}|
$$

当两个向量$\mathbf{a}$、$\mathbf{b}$的大小固定时，根据柯西-施瓦茨不等式，可以得出以下事实：

- 当两个向量方向相反时候，内积取得最小值。
- 当两个向量不平行时，内积取值取平行时的中间值。
- 当两个向量方法相同时候，内积取得最大值。

#### 内积的坐标表示

在平面中向量内积可以表示如下：

当$\mathbf{a} = (a_1, a_2), \mathbf{b} = (b_1,b_2)$时，$\mathbf{a}\cdot\mathbf{b} = a_1b_1 + a_2b_2$

在三维空间中，内积的坐标表示如下所示：

当$\mathbf{a} = (a_1,a_2, a_3)$,$\mathbf{b} = (b_1,b_2,b_3)$时，$\mathbf{a}\cdot\mathbf{b} = a_1b_1 + a_2b_2 + a_3b_3$。

推广到任意的$n$维空间，有：

对于两个向量$\mathbf{a} = (a_1, a_2, \cdot, a_n)$,$\mathbf{b} = (b_1,b_2,\cdot,b_n)$,其内积$\mathbf{a}\cdot\mathbf{b}$如下式所示：

$$
\mathbf{a}\cdot\mathbf{a} = a_1b_1 + a_2b_2 + \cdot + a_nb_n
$$

### 矩阵

矩阵是数的阵列。

$$
A = 
\left (
\begin{matrix}
3 & 1 & 4\\
1 & 5 & 9 \\
2 & 6 & 5 \\
\end{matrix}
\right )
$$

在上式中A是3行3列构成的矩阵。若行数与列数相同的矩阵称为方阵。

如下所示的矩阵$X$、$Y$分别称为列向量、行向量，也可简称为向量。

$$
X = 
\left (
\begin{matrix}
3 \\
1 \\
4
\end{matrix}
\right ), Y = \left (
\begin{matrix}
2 & 7 & 1
\end{matrix}
\right )
$$

我们将矩阵$A$推广到更一般的情形，如下所示：

$$
\begin{split}\mathbf{A}=\left (\begin{matrix} a_{11} & a_{12} & \cdots & a_{1n} \\ a_{21} & a_{22} & \cdots & a_{2n} \\ \vdots & \vdots & \ddots & \vdots \\ a_{m1} & a_{m2} & \cdots & a_{mn} \\ \end{matrix}\right )\end{split}
$$

上面是一个$m$行$n$列的矩阵。位于第$i$行第$j$列的值（称为元素）用$a_{ij}$表示。

#### 单位矩阵

若一个方阵，它对角线上的元素$a_{ii}$为1，其他元素为0，我们称为单位矩阵，通常用$E$表示。

> [!note] 笔记
> $E$为德语中表示为1的单词Ein的首字母

#### 矩阵相等

两个矩阵$A$、$B$相等的含义是它们对应的元素相等，记为$A=B$。

#### 矩阵的和、差、常数倍

两个矩阵$A$、$B$的和$A+B$、差$A-B$定义为相同位置的元素的和、差所产生的矩阵。矩阵的常数倍定义为各个元素常数倍所产生的矩阵。

#### 矩阵的乘积

对于两个矩阵$A$、$B$,将$A$的第$i$行看做行向量，B的$j$列看做列向量，将它们的内积作为第$i$行第$j$列元素，由此产生的矩阵就是矩阵$A$、$B$的乘积$AB$。

![](https://static.cyub.vip/images/202507/epub_27337415_506.jpg)

单位矩阵$E$与任意矩阵$A$的乘积都满足交换律，即；

$$
AE = EA = A
$$

而其他矩阵间的乘法不满足交换律，即：

$$
AB \ne BA
$$

#### Hadamard乘积

对于相同形状的矩阵$A$、$B$，将相同位置的元素相乘，由此产生的矩阵称为矩阵$A$、$B$的 Hadamard 乘积，用$A \odot B$表示。

例如，当$A = \left (
\begin{matrix}
2 & 7\\
1 & 8
\end{matrix}
\right )$, $B = \left (
\begin{matrix}
2 & 8\\
1 & 3
\end{matrix}
\right )$时：

$$
A\odot B = \left (
\begin{matrix}
2\cdot 2 & 7 \cdot 8\\
1 \cdot 1 & 8 \cdot 3
\end{matrix}
\right ) = \left (
\begin{matrix}
4 & 56\\
1 & 24
\end{matrix}
\right )
$$

#### 转置矩阵

将矩阵$A$的第$i$行第$j$列的元素与第$j$行第$i$列的元素交换，由此产生的矩阵称为矩阵$A$的转置矩阵（transposed matrix)，记作$A^T$。

### 导数

#### 定义

函数$y=f(x)$导函数$f'(x)$的定义如下所示：

$$
f'(x) = \lim_{\Delta x-> 0}\frac{f(x + \Delta x) - f(x)}{\Delta x} \quad{(1)}
$$

上面式子中希腊字母$Delta$读作$delta$，对应拉丁字母$D$，此外，带有$'$(prime)符号的函数或者变量表示导函数。

已知函数$f(x)$，求导函数$f'(x)$，称为对函数$f(x)$求导，当式(1)的值存在时，称函数可导。

导函数的几何含义下图所示：

![](https://static.cyub.vip/images/202507/epub_27337415_559.jpg)

函数$y=f(x)$的导函数用$f'(x)$表示外，也可以使用分数形式表示，两者是等效的:

$$
f'(x) = \frac{dy}{dx}
$$

#### 导数的性质

$$
{f(x) + g(x)}' = f'(x) + g'(x)
$$

$$
{cf(x)}' = cf'(x)\quad{（c为常数）}
$$

上面性质可以总结为**和的导数为导数的和，常数倍的导数为导数的常数倍**。

当函数时分数形式时候，存在下面的求导公式

$$
\{\frac{1}{f(x)}\}' = -\frac{f'(x)}{\{f(x)\}^2}
$$

上面公式中$f(x)$不取0值。

##### Sigmoid函数的导数

Sigomod函数的定义如下所示：

$$
\sigma(x) = \frac{1}{1 + e^{-x}}
$$

它的导数为：

$$
\sigma'(x) = \sigma(x)(1- \sigma(x))
$$

证明：将$1+e^{-x}$带入上面分数形式的函数导数公式中，我们得到：

$$
\begin{aligned}
\sigma'(x) &= - \frac{(1+ e^{-x})'}{(1+e^{-x})^2} \\
&= \frac{e^{-x}}{(1+ e^{-x})^2}\\
&= \frac{1 + e^{-x} -1}{(1+e^{-x})^2} \\
&= \frac{1}{1 + e^{-x}} - \frac{1}{(1+ e^{-x})^2}\\
&= \sigma(x) - \sigma(x)^2\\
&= \sigma(x)(1- \sigma(x))
\end{aligned}
$$

#### 最小值条件

当函数$f(x)$在$x=a$处取得最小值时，$f'(a) = 0$。从下图中，我们可以看出$f'(a) = 0$是函数$f(x)$在$x = a$处取得最小值的必要条件,但不是充条件。

![](https://static.cyub.vip/images/202507/epub_27337415_611.jpg)

> [!note] 笔记
> 已知命题$p$、$q$,由$p$可以推出$q$，则$q$称为$p$的必要条件。

### 偏导数

#### 多变量函数

有两个以上的自变量的函数称为多变量函数。单变量函数用$f(x)$表示，同理我们可以用$f(x, y)$表示有两个自变量$x$,$y$的函数。以此类推，由$n$个自变量$x_1$,$x_2$,$\cdots$,$x_n$的函数，可以记作$f(x_1,x_2,\cdots,x_n)$。

#### 偏导数

关于某个特定变量的导数，称为偏导数（partial derivative)。

考虑有两个变量$x$、$y$的函数$z = f(x, y)$,关于$x$的偏导数可以用下面符号表示:

$$
\frac{\partial z}{\partial x} = \frac{\partial f(x,y)}{\partial x} = \lim_{\Delta x\to 0}\frac{f(x + \Delta x, y) - f(x,y）}{\Delta x}
$$

同样关于$y$的偏导数如下表示：

$$
\frac{\partial z}{\partial y} = \frac{\partial f(x,y)}{\partial y} = \lim_{\Delta y\to 0}\frac{f(x , y+\Delta y) - f(x,y)}{\Delta y}
$$

#### 多变量函数的最小值条件

对于函数$z = f(x, y)$取得最小值的必要条件是$\frac{\partial f}{\partial x} = 0, \frac{\partial f}{\partial y} = 0$。

例子： 求函数$z = x^2 + y ^2$取得最小值时$x$,$y$的值。

首先，我们来求关于$x$,$y$的偏导数。

$$
\frac{\partial z}{\partial x} = 2x, \frac{\partial z}{\partial y} = 2y
$$

根据多变量函数取得最小值的的必要条件，该函数取得最小值的必要条件是$x=0, y=0$，此时函数值$z$为0。由于$z= x^2 + y^2 \ge 0$,所以可以得出这个函数值0就是该函数的最小值。


例子： 当$x^2 + y^2 = 1$时，求$x+y$的最小值。

在实际的最小值问题中，有时会对变量附加约束条件，比如这个题目。这种情况下我们可以使用拉格朗日乘数法。这个方法首先引入参数$\lambda$,创建下面的函数$\mathcal{L}$。

$$
\mathcal{L} = f(x, y) - \lambda g(x, y) = (x + y) - \lambda(x^2 + y^2 -1)
$$

利用多元函数最小值必要条件，我们得到：

$$
\frac{\partial \mathcal{L} }{\partial x} = 1 - 2\lambda x = 0, \frac{\partial \mathcal{L} }{\partial y} = 1 - 2\lambda y = 0,
$$

根据上面式子，以及约束条件$x^2 + y^2 = 1$，我们计算得到$x = y = \lambda = \pm\frac{1}{\sqrt{2}}$。

因此，当$x = y = -\frac{1}{\sqrt{2}}$时，$x+y$取得最小值$-2$。

#### 链式法则

##### 复合函数

已知函数$y = f(u)$,当$u$表示为$u = g(x)$时，$y$作为$x$的函数可以表示为形如$y = f(g(x))$的嵌套函数（$u$和$x$表示多变量）。这时，嵌套结构的函数$f(g(x))$称为$f(u)$和$g(x)$的复合函数。

例1：函数$z = (2-y)^2$是函数$u = 2 - y$和函数$z = u^2$的复合函数。

![](https://static.cyub.vip/images/202507/epub_27337415_708.jpg)

例2：对于多个输入$x_1,x_2,\cdots,x_n$,将$a(x)$作为激活函数，求神经单元的输出$y$的过程如下表示：

$$
y = a(w_1x_1 + w_2x_2 + \cdots + w_nx_n + b)
$$

$w_1,w_2,\cdots,w_n$为各输入对应的权重，$b$为神经单元的偏置。这个输出函数是如下的$x_1,x_2,\cdots,x_n$的一次函数$f$和激活函数$a$的复合函数。

$$
\left \{
\begin{array}{l} % l 靠左、c 居中、r 靠右
z = f(x_1, x_2, \cdots, x_n) = w_1x_1 + w_2x_2 + \cdots + w_nx_n  +b\\\\
y = a(z)
\end{array}
\right.
$$

![](https://static.cyub.vip/images/202507/epub_27337415_719.jpg)

##### 单变量函数的链式法则

已知单变量函数$y = f(u)$，当$u$表示为单变量函数$u = g(x)$时，复合函数$f(g(x))$的导函数公式如下：

$$
\frac{dy}{dx} = \frac{dy}{du}\frac{du}{dx}
$$

这个公式称为单变量函数的复合函数求导公式，也称为链式法则。

![](https://static.cyub.vip/images/202507/epub_27337415_725.jpg)

例3：当$y$为$u$的函数，$u$为$v$的函数，$v$为$x$的函数时，那么:

$$
\frac{dy}{dx} = \frac{dy}{du}\frac{du}{dv}\frac{dv}{dx}
$$

![](https://static.cyub.vip/images/202507/epub_27337415_740.jpg)

示例：对于$x$的函数$y = \frac{1}{1 + e^{-(wx + b)}}(w, b为常数)$求导.

我们设定一下函数：

$$
y = \frac{1}{1+ e^{-u}}, \quad u = wx +b
$$

有前面的Sigmod函数的导函数公式可以得出：

$$
\frac{dy}{du} = y(1-y)
$$

此外,由于$\frac{du}{dx} =w$,所以可得：

$$
\frac{dy}{dx} = \frac{dy}{du}\frac{du}{dx} = y(1-y)w = \frac{w}{1 + e^{-(wx+b)}}(1 - \frac{1}{1+ e^{-(wx+b)}})
$$

##### 多变量函数的链式法则

在多变量函数的情况下，链式法则同样适用，但必须对相关的全部变量应用链式法则。

考察两个变量的情况。变量$z$为$u$,$v$的函数，如果$u$,$v$分别为$x$,$y$的函数，则$z$为$x$,$y$的函数，此时有下面公式：

$$
\frac{\partial z}{\partial x} = \frac{\partial z}{\partial u}\frac{\partial u}{\partial x} + \frac{\partial z}{\partial v}\frac{\partial v}{\partial x}
$$

![](https://static.cyub.vip/images/202507/epub_27337415_760.jpg)

同理对$y$求导，也有下面公式：

$$
\frac{\partial z}{\partial y} = \frac{\partial z}{\partial u}\frac{\partial u}{\partial y} + \frac{\partial z}{\partial v}\frac{\partial v}{\partial y}
$$

上面公式中各变量的关系图如下：

![](https://static.cyub.vip/images/202507/epub_27337415_762.jpg)
例5：当$C = u^2 + v^2, u= ax+ by, v = px +qy\quad{a,b,p,q为常数}$时，求函数$C$分别关于$x$和$y$的偏导函数。

$$
\begin{aligned}
\frac{\partial C}{\partial x} &= \frac{\partial C}{\partial u}\frac{\partial u}{\partial x} + \frac{\partial C}{\partial v}\frac{\partial v}{\partial x} \\\\
&= 2u \cdot a + 2v \cdot p \\\\
&= 2a(ax+ by) +2p(px + qy)
\end{aligned}
$$

$$
\begin{aligned}
\frac{\partial C}{\partial y} &= \frac{\partial C}{\partial u}\frac{\partial u}{\partial y} + \frac{\partial C}{\partial v}\frac{\partial v}{\partial y} \\\\
&= 2u \cdot b + 2v \cdot q \\\\
&= 2b(ax+ by) +2q(px + qy)
\end{aligned}
$$