# 03 — 树模型与集成学习（Tree Models & Ensemble Learning）

> 树模型（Decision Trees）是机器学习中最直观、最可解释的算法之一。但单棵决策树容易过拟合（overfitting /ˈoʊvərˈfɪtɪŋ/）、不稳定（数据微小变化会导致树结构巨变）。集成学习（Ensemble Learning）通过组合多个弱学习器来构建强学习器，有效解决了这些问题。
> > **时间线**:
> > - **1963**: Vapnik & Chervonenkis 提出 VC 维理论
> > - **1984**: Breiman 等人发表 CART（Classification and Regression Trees）
> > - **1986**: Quinlan 提出 ID3 决策树算法
> > - **1997**: Freund & Schapire 发表 AdaBoost 算法
> - **2001**: Breiman 提出随机森林（Random Forest）
>
> 本章涵盖两大集成范式：**Bagging**（以随机（stochastic /stəˈkæstɪk/）森林为代表）和 **Boosting**（以 GBDT / XGBoost 为代表）。你将看到为什么随机森林能降低方差而不增偏差，以及 XGBoost 为何成为 Kaggle 竞赛和工业界表格数据的常胜将军。
>
> 章节路线：**决策树 → Bagging → 随机森林 → Boosting → GBDT → XGBoost → 树 vs 深度学习**

---

## 1. 决策树（Decision Trees）

### 1.1 树结构

决策树模拟人类做决策的过程：从根节点（Root Node）开始，根据特征值做判断，沿着分支到达内部节点（Internal Nodes），最终到达叶节点（Leaf Nodes），每个叶节点对应一个预测值（分类（classification /ˌklæsɪfɪˈkeɪʃən/）或回归（regression /rɪˈɡreʃən/））。

```
                    [Outlook = Sunny?]
                    /                \
                 Yes                  No
                 /                      \
        [Humidity > 75%?]           [Outlook = Overcast?]
          /          \                 /            \
        Yes           No             Yes             No
        /              \             /                \
    [Play=No]      [Play=Yes]   [Play=Yes]    [Wind = Strong?]
                                                  /          \
                                                Yes           No
                                                /              \
                                           [Play=No]      [Play=Yes]
```

### 1.2 如何选择分裂特征：信息增益 vs Gini 不纯度

决策树的核（kernel /ˈkɜːrnl/）心问题：**在每个节点，选择哪个特征、以什么阈值分裂，能使子节点最"纯"？**

#### 信息增益（Information Gain）

从第 04 章的信息论我们知道，熵（entropy /ˈentrəpi/）（Entropy）衡量不确定性。分裂后子节点的加权熵与父节点熵的差值，就是**信息增益**：

$$IG(D, a) = H(D) - \sum_{v \in Values(a)} \frac{|D_v|}{|D|} H(D_v)$$

其中 $H(D) = -\sum_{k} p_k \log p_k$ 是数据集 $D$ 的熵，$p_k$ 是第 $k$ 类的比例。

**直觉**：信息增益越大，说明这个特征分裂后不确定性降低得越多——即这个特征越重要。

```python
def entropy(labels):
    """计算标签的熵"""
    _, counts = np.unique(labels, return_counts=True)
    probs = counts / counts.sum()
    return -np.sum(probs * np.log2(probs + 1e-10))

def info_gain(data, labels, feature_idx, threshold):
    """计算按某特征阈值分裂的信息增益"""
    parent_entropy = entropy(labels)
    left_mask = data[:, feature_idx] <= threshold
    right_mask = ~left_mask
    n = len(labels)
    n_left, n_right = left_mask.sum(), right_mask.sum()
    if n_left == 0 or n_right == 0:
        return 0
    child_entropy = (n_left / n) * entropy(labels[left_mask]) + \
                    (n_right / n) * entropy(labels[right_mask])
    return parent_entropy - child_entropy
```

#### Gini 不纯度（Gini Impurity）

Gini 不纯度衡量从数据集中**随机抽取两个样本，其类别不一致的概率**：

$$Gini(D) = 1 - \sum_{k} p_k^2$$

- 所有样本属于同一类：$Gini = 0$（最纯）
- 类别均匀分布（$K$ 类）：$Gini = 1 - 1/K$（最不纯）

分裂时选择使**加权 Gini 不纯度最小化**的分裂方式。

**信息增益 vs Gini**：

| 指标 | 公式 | 取值范围 | 偏好 |
|:---|:---|:---:|:---|
| 信息增益 | $H(D) - \sum \frac{n_v}{n} H(D_v)$ | $[0, \log K]$ | 偏向多值特征 |
| Gini 不纯度 | $1 - \sum p_k^2$ | $[0, 1-1/K]$ | 计算更快，通常与信息增益结果相似 |

> **实践建议**：sklearn 的 `DecisionTreeClassifier` 默认使用 `criterion='gini'`。两者在实际应用中差异不大，Gini 计算稍快。

### 1.3 过拟合与剪枝

决策树的一个严重问题是**过拟合**：如果不加限制，树可以不断分裂直到每个叶节点只包含一个样本（训练精度 100%），但泛化能力很差。

**过拟合表现**：
- 树的深度很大
- 叶节点样本数很少
- 训练精度远高于测试精度

**缓解方法**：

| 方法 | 描述 |
|:---|:---|
| **预剪枝（Pre-pruning）** | 在树生长过程中提前停止：限制 `max_depth`、`min_samples_split`、`min_samples_leaf` |
| **后剪枝（Post-pruning）** | 先让树充分生长，再自底向上合并不显著的分支（CCP，Cost-Complexity Pruning） |
| **限制叶节点最小样本数** | 每个叶节点至少包含 $N$ 个样本，防止"记忆"噪声 |

> 详见配套代码 `tree_ensemble.py` 中的 `compare_trees()`，该函数对比了不同深度下决策树在 Iris 数据集上的表现。

### 1.4 代码精读：决策树到底"长"成什么样？

调用 `DecisionTreeClassifier().fit()` 之后，sklearn 把整棵树压平存进 `tree.tree_` 的几个**平行数组**里，节点用整数下标编号（0 是根）。理解这几个数组，你就能脱离"黑盒"，亲手读出每个分裂判断——这正是 `tree_ensemble.py` 里 `_tree_to_text()` 做的事：

```python
def _tree_to_text(tree, feature_names, class_names, lines, node=0, depth=0):
    indent = "  " * depth
    n_samples = tree.tree_.n_node_samples[node]   # 该节点覆盖多少训练样本
    value = tree.tree_.value[node][0]             # 各类别的样本计数 [n0, n1, n2]
    class_idx = np.argmax(value)                  # 多数类 = 该节点的预测类别
    class_name = class_names[class_idx]

    # 判断是不是叶子：sklearn 用"左右孩子相同"(都为 -1)来标记叶节点
    if tree.tree_.children_left[node] == tree.tree_.children_right[node]:
        lines.append(f"{indent}├── Leaf: class={class_name}, samples={n_samples}, ...")
    else:
        feature = feature_names[tree.tree_.feature[node]]  # 该节点用哪个特征分裂
        threshold = tree.tree_.threshold[node]             # 分裂阈值
        impurity = tree.tree_.impurity[node]               # 该节点的 Gini/熵
        lines.append(f"{indent}├── [{feature} <= {threshold:.2f}] gini={impurity:.3f} ...")
        # 递归：先左子树(特征 <= 阈值)，再右子树(特征 > 阈值)
        _tree_to_text(..., tree.tree_.children_left[node],  depth + 1)
        _tree_to_text(..., tree.tree_.children_right[node], depth + 1)
```

逐行对应到**业务含义**：

| 代码 | 数组含义 | 业务解读 |
|:---|:---|:---|
| `tree_.feature[node]` | 该内部节点选用的特征下标 | 模型认为"这一步最该问"的问题，例如"花瓣长度是多少？" |
| `tree_.threshold[node]` | 分裂阈值 | 问题的判定线："花瓣长度 ≤ 2.45cm 吗？" |
| `tree_.value[node]` | 落入该节点的各类别样本数 | 当前已经把样本筛到什么纯度；`argmax` 就是该叶子的预测 |
| `tree_.impurity[node]` | 节点不纯度（默认 Gini） | 对应 §1.2 的 $Gini(D)$，越接近 0 说明这一支越"干净" |
| `children_left/right == -1` | 没有孩子 | 叶节点，停止提问、直接给预测 |

**关键点**：`children_left` 对应"特征 $\le$ 阈值"的样本，`children_right` 对应"$>$ 阈值"。所以正文那棵 Iris 决策树之所以能 100% 分出 setosa，就是因为根节点一刀 `花瓣长度 ≤ 2.45` 把 setosa 全部切到左边（叶子的 `value` 形如 `[50, 0, 0]`，Gini=0）。`fit()` 内部做的，正是 §1.2 那套"枚举每个特征 × 每个阈值，挑使加权不纯度最小的分裂"，只是用 C 实现得很快而已。

---

## 2. 随机森林（Random Forest）

### 2.1 Bagging 原理

**Bagging**（Bootstrap Aggregating）的核心思想：

1. 从原始数据集 $D$ 中**有放回地采样** $N$ 次，生成一个 Bootstrap 样本 $D_i$（大小与原始数据集相同，但约有 $63.2\%$ 的原始样本出现，其余为重复）
2. 在 $D_i$ 上训练一个基学习器（如决策树）
3. 重复 $T$ 次，得到 $T$ 个模型
4. **回归**：取 $T$ 个预测的平均值；**分类**：取 $T$ 个预测的投票结果

::: details 推导：为什么 Bootstrap 样本只覆盖约 63.2% 的原始数据？
数据集有 $N$ 个样本，每次有放回抽样时，某个固定样本 $x_i$ **没被抽中**的概率是 $1 - \frac{1}{N}$。抽 $N$ 次都没抽中的概率为

$$P(x_i \text{ 从未被抽中}) = \left(1 - \frac{1}{N}\right)^N$$

利用极限 $\lim_{N\to\infty}\left(1-\frac{1}{N}\right)^N = e^{-1}$，当 $N$ 较大时

$$P(\text{未抽中}) \approx e^{-1} \approx 0.368 \quad\Rightarrow\quad P(\text{至少被抽中一次}) \approx 1 - 0.368 = 0.632$$

所以每个 Bootstrap 样本平均覆盖 $63.2\%$ 的不同原始样本，剩下 $36.8\%$ 没被抽到——它们正是下面 §2.4 要用的 **OOB 样本**。
:::

```python
def bagging_predict(models, X):
    """Bagging 集成预测"""
    predictions = np.array([model.predict(X) for model in models])
    # 分类：投票（每行是一个样本，每列是一个模型）
    from scipy.stats import mode
    return mode(predictions, axis=0)[0].ravel()
```

### 2.2 为什么 Bagging 能降低方差？

假设 $T$ 个基学习器的方差均为 $\sigma^2$，两两之间的相关系数为 $\rho$，则集成的方差为：

$$\text{Var}(\text{ensemble}) = \rho \sigma^2 + \frac{1 - \rho}{T} \sigma^2$$

::: details 推导：集成方差公式从哪来？
集成预测是 $T$ 棵树预测的平均：$\bar{f} = \frac{1}{T}\sum_{i=1}^T f_i$。用方差的基本公式展开（方差对常数 $\frac{1}{T}$ 平方提出，协方差展开所有两两组合）：

$$\text{Var}(\bar f) = \frac{1}{T^2}\,\text{Var}\!\Big(\sum_{i=1}^T f_i\Big) = \frac{1}{T^2}\Big(\underbrace{\sum_{i} \text{Var}(f_i)}_{T \text{ 个对角项}} + \underbrace{\sum_{i\neq j}\text{Cov}(f_i,f_j)}_{T(T-1) \text{ 个非对角项}}\Big)$$

代入假设：每棵树方差相同 $\text{Var}(f_i)=\sigma^2$，两两相关系数为 $\rho$，即 $\text{Cov}(f_i,f_j)=\rho\sigma^2$：

$$\text{Var}(\bar f) = \frac{1}{T^2}\Big(T\sigma^2 + T(T-1)\rho\sigma^2\Big) = \frac{\sigma^2}{T} + \frac{T-1}{T}\rho\sigma^2$$

把 $\frac{T-1}{T}=1-\frac{1}{T}$ 代入并整理，就得到正文的形式：

$$\text{Var}(\bar f) = \rho\sigma^2 + \frac{1-\rho}{T}\sigma^2$$

**读这个式子**：第一项 $\rho\sigma^2$ 与树的数量 $T$ 无关——这是无论加多少棵树都消不掉的"地板"，只能靠**降低树之间的相关性 $\rho$** 来压低（这正是随机森林做特征随机化的动机）；第二项 $\frac{1-\rho}{T}\sigma^2$ 随 $T\to\infty$ 趋于 0，这是"多加树"能拿到的收益。
:::

- 当 $\rho = 0$（模型完全独立）：方差降至 $\sigma^2 / T$（理想情况，但不可实现）
- 当 $\rho = 1$（模型完全一样）：方差仍为 $\sigma^2$（Bagging 无效果）

决策树对数据扰动敏感，不同 Bootstrap 样本上的树差异很大（$\rho$ 较小），因此方差能有效降低。

**重要性质**：Bagging **不增加偏差**。因为每棵树的偏差相同，平均后偏差不变。

### 2.3 随机森林的特征随机性

随机森林在 Bagging 的基础上增加了一层随机性：**在每个节点分裂时，只考虑特征的一个随机子集**。

- **分类**：通常考虑 $\sqrt{d}$ 个特征（$d$ 为总特征数）
- **回归**：通常考虑 $d/3$ 个特征

这进一步降低了树之间的相关性（$\rho$ 更小），从而更有效地降低方差。

```python
# 随机森林 vs 单棵决策树
rf = RandomForestClassifier(n_estimators=100, max_features='sqrt', random_state=42)
tree = DecisionTreeClassifier(random_state=42)
```

> 详见配套代码 `tree_ensemble.py` 中的 `compare_rf_vs_tree()`，该函数对比了随机森林和单棵决策树在 Iris 上的准确性差异，并展示了特征重要性。

### 2.4 OOB 误差（Out-of-Bag Error）

每个 Bootstrap 样本中，约有 $36.8\%$ 的样本未被抽中，这些样本称为 **OOB 样本**。可以用 OOB 样本评估模型性能，无需单独的验证集：

$$\text{OOB Error} = \frac{1}{N} \sum_{i=1}^N \mathbb{I}(y_i \neq \hat{y}_i^{\text{OOB}})$$

其中 $\hat{y}_i^{\text{OOB}}$ 是在所有未包含样本 $i$ 的树上投票得到的预测。

---

## 3. GBDT / XGBoost

### 3.1 Boosting vs Bagging

| | Bagging | Boosting |
|:---|:---|:---|
| **训练方式** | 并行训练多个模型 | 串行训练，每个模型纠正前一个的错误 |
| **目标** | 降低方差 | 同时降低偏差和方差 |
| **基学习器** | 强学习器（深树） | 弱学习器（浅树，通常 1-3 层） |
| **样本权重** | 均匀采样 | 动态调整，加大错分样本权重 |

### 3.2 Gradient Boosting 直觉

Gradient（/ˈɡreɪdiənt/） Boosting 的核心思想：**每一棵新树拟合的是前面所有树的残差（Residuals）**。

**逐步推导**：

1. 初始化：$\hat{y}^{(0)} = \bar{y}$（常数预测）
2. 计算残差：$r_i = y_i - \hat{y}_i^{(0)}$
3. 训练一棵树 $h_1(x)$ 拟合残差 $r_i$
4. 更新：$\hat{y}_i^{(1)} = \hat{y}_i^{(0)} + \eta \cdot h_1(x_i)$（$\eta$ 是学习率）
5. 重复：每一步的残差 $r_i = y_i - \hat{y}_i^{(t-1)}$，新树 $h_t$ 拟合这个残差

**为什么叫"Gradient" Boosting？**

残差 $y_i - \hat{y}_i$ 实际上是**均方误差损失函数的负梯度方向**：

$$\frac{\partial}{\partial \hat{y}_i} \frac{1}{2}(y_i - \hat{y}_i)^2 = -(y_i - \hat{y}_i)$$

所以拟合残差 = 沿着负梯度方向更新，等价于**梯度下降**——只不过是在函数空间而非参数（parameter /pəˈræmɪtər/）空间进行的梯度下降。

```python
# 伪代码：Gradient Boosting
def gradient_boosting(X, y, n_estimators=100, lr=0.1):
    models = []
    residual = y.copy()
    for _ in range(n_estimators):
        tree = DecisionTreeRegressor(max_depth=3)
        tree.fit(X, residual)        # 拟合残差
        residual -= lr * tree.predict(X)  # 更新残差
        models.append(tree)
    return models
```

**逐行精读这段"手写 GBDT"**（它把 §3.2 的数学翻译成了循环）：

- `residual = y.copy()`：第 0 棵树之前，"还没解释的部分"就是真实值本身（等价于初始预测为 0）。`residual` 这个变量在整个循环里扮演**当前还没拟合掉的误差**。
- `tree.fit(X, residual)`：注意第二个参数不是 `y` 而是 `residual`——**新树学的不是原始标签，而是上一轮剩下的残差**。这是 Boosting 与 Bagging 最本质的代码差异（Bagging 里每棵树都 `fit(X, y)`）。
- `residual -= lr * tree.predict(X)`：把这棵新树的贡献（乘以学习率 `lr`）从残差里扣掉，剩下的就是交给下一棵树的新任务。`lr<1` 让每棵树只"迈一小步"，避免一步走过头——对应 XGBoost 的 Shrinkage。
- 预测时（未在伪代码中）：把所有树的输出按 `lr` 加权累加，$\hat y = \sum_t \eta\, h_t(x)$。

把它和 §3.2 的负梯度推导对照：因为 MSE 的负梯度恰好等于残差 $y-\hat y$，所以"`fit(残差)`"这一行在数学上就是"沿损失函数的负梯度方向，在函数空间里走一步梯度下降"。XGBoost 只是把这里的一阶残差换成上面推导的 $w_j^{*}=-G_j/(H_j+\lambda)$（用上了二阶信息 $H_j$ 和正则 $\lambda$），其余循环骨架完全一致。

### 3.3 XGBoost 的三大创新

XGBoost（eXtreme Gradient Boosting）是 GBDT 的高效工程实现，在 Kaggle 竞赛中长期占据统治地位。

#### 创新 1：正则化目标函数

传统的 GBDT 只最小化损失函数，XGBoost 加入了模型复杂度惩罚项：

$$\text{Obj} = \sum_{i=1}^n L(y_i, \hat{y}_i) + \sum_{t=1}^T \Omega(f_t)$$

其中 $\Omega(f) = \gamma T + \frac{1}{2}\lambda \sum_{j=1}^T w_j^2$

- $T$：叶节点数（控制树的复杂度）
- $w_j$：叶节点权重值
- $\gamma, \lambda$：正则化（regularization /ˌreɡjələraɪˈzeɪʃən/）超参数（hyperparameter /ˈhaɪpərpəˈræmɪtər/）

这相当于在 GBDT 基础上加了 **L2 正则化**（类似 Ridge Regression），有效防止过拟合。

#### 创新 2：二阶泰勒近似

传统 GBDT 使用一阶梯度（残差），XGBoost 使用**二阶泰勒展开**来近似损失函数：

$$L(y, \hat{y}^{(t)}) \approx L(y, \hat{y}^{(t-1)}) + g_i f_t(x_i) + \frac{1}{2} h_i f_t^2(x_i)$$

其中 $g_i = \partial_{\hat{y}} L(y_i, \hat{y}^{(t-1)})$ 是一阶梯度，$h_i = \partial_{\hat{y}}^2 L(y_i, \hat{y}^{(t-1)})$ 是二阶梯度。

使用二阶信息使优化更精准、收敛更快。

::: details 完整推导：最优叶子权重 $w_j^{*}$ 与结构分数 / 分裂增益
这是 XGBoost 论文最核心的一步，也是大多数教程"一带而过"的地方。我们一步步把它推完。

**第 1 步：把目标函数写成只关于第 $t$ 棵树的形式。** 在第 $t$ 轮，前 $t-1$ 棵树已固定，常数项 $L(y_i,\hat y^{(t-1)})$ 对优化没有影响，丢掉它。再把正则项 $\Omega(f_t)=\gamma T+\frac{1}{2}\lambda\sum_j w_j^2$ 代入：

$$\text{Obj}^{(t)} \approx \sum_{i=1}^n \Big[g_i f_t(x_i) + \tfrac{1}{2} h_i f_t^2(x_i)\Big] + \gamma T + \frac{1}{2}\lambda\sum_{j=1}^T w_j^2$$

**第 2 步：从"按样本求和"换成"按叶子求和"。** 一棵树把每个样本映射到唯一的叶子，叶子 $j$ 的权重是 $w_j$，落在叶子 $j$ 里的样本集合记作 $I_j=\{i \mid q(x_i)=j\}$。对叶子 $j$ 里的所有样本，$f_t(x_i)$ 都等于同一个值 $w_j$。于是把求和按叶子分组：

$$\text{Obj}^{(t)} = \sum_{j=1}^T \Big[\Big(\sum_{i\in I_j} g_i\Big)w_j + \frac{1}{2}\Big(\sum_{i\in I_j} h_i + \lambda\Big)w_j^2\Big] + \gamma T$$

记 $G_j=\sum_{i\in I_j} g_i$（叶子内一阶梯度之和）、$H_j=\sum_{i\in I_j} h_i$（二阶梯度之和），写成：

$$\text{Obj}^{(t)} = \sum_{j=1}^T \Big[G_j w_j + \frac{1}{2}(H_j+\lambda)w_j^2\Big] + \gamma T$$

**第 3 步：对每个叶子权重求极值。** 树结构固定时，各叶子的 $w_j$ 互不影响，是 $T$ 个**独立的一元二次函数**。每个形如 $a w + \frac{1}{2}b w^2$（其中 $a=G_j$，$b=H_j+\lambda>0$），开口向上，对 $w_j$ 求导令其为零：

$$\frac{\partial}{\partial w_j}\Big[G_j w_j + \tfrac{1}{2}(H_j+\lambda)w_j^2\Big] = G_j + (H_j+\lambda)w_j = 0 \;\Rightarrow\; \boxed{w_j^{*} = -\frac{G_j}{H_j+\lambda}}$$

**第 4 步：回代得到"结构分数"。** 把 $w_j^{*}$ 代回，单个叶子的最小值是 $G_j w_j^{*} + \frac{1}{2}(H_j+\lambda)w_j^{*2} = -\frac{G_j^2}{H_j+\lambda}+\frac{1}{2}\frac{G_j^2}{H_j+\lambda} = -\frac{1}{2}\frac{G_j^2}{H_j+\lambda}$。求和得到这棵树结构的"分数"（越小越好）：

$$\text{Obj}^{*} = -\frac{1}{2}\sum_{j=1}^T \frac{G_j^2}{H_j+\lambda} + \gamma T$$

这就是 **结构分数（Structure Score）**——它只依赖每个叶子内的 $G_j,H_j$，给任意一棵候选树打一个"好坏分"。

**第 5 步：分裂增益。** 建树时无法枚举所有结构，于是用贪心：考虑把一个叶子分成左右两个叶子（梯度和分别为 $G_L,H_L$ 和 $G_R,H_R$，且 $G_L+G_R=G$）。分裂前后结构分数之差（分裂带来的目标下降量）就是**分裂增益**：

$$\text{Gain} = \underbrace{\frac{1}{2}\Big[\frac{G_L^2}{H_L+\lambda} + \frac{G_R^2}{H_R+\lambda} - \frac{(G_L+G_R)^2}{H_L+H_R+\lambda}\Big]}_{\text{纯度提升}} - \underbrace{\gamma}_{\text{新增一个叶子的代价}}$$

XGBoost 在每个候选分裂点计算这个 Gain，选最大的；如果所有分裂的 Gain 都 $<0$（即纯度提升抵不过 $\gamma$ 的代价），就停止分裂——这正是 $\gamma$ 作为**预剪枝**阈值的作用。至此，§3.3 的三大创新里"正则化 + 二阶梯度"如何具体决定建树，全部闭环。
:::

#### 创新 3：近似贪心算法（Approximate Greedy Algorithm）

当数据量很大时，遍历所有可能的分裂点不现实。XGBoost 使用**分位数近似**：

- 将连续特征分桶（如分成 100 个桶）
- 只在桶边界上评估分裂增益
- 支持加权分位数（Weighted Quantile Sketch），使损失大的样本获得更多"注意力（attention /əˈtenʃən/）"

此外，XGBoost 还包含：

| 特性 | 作用 |
|:---|:---|
| **列采样（Column Subsampling）** | 类似随机森林，降低过拟合 |
| **Shrinkage（学习率）** | 每棵树贡献乘以 $\eta$，留更多空间给后续树 |
| **并行化** | 在特征级别并行（树本身仍是串行的） |
| **处理缺失值** | 自动学习缺失值的最佳分裂方向 |

```python
# XGBoost 使用示例
model = XGBClassifier(n_estimators=100, max_depth=3, learning_rate=0.1,
                      reg_lambda=1.0, subsample=0.8, colsample_bytree=0.8,
                      eval_metric='logloss', random_state=42)
model.fit(X_train, y_train)
```

> 详见配套代码 `tree_ensemble.py` 中的 `compare_all_models()`，该函数在 Iris 数据集上对比了决策树、随机森林和 XGBoost 的精度。

---

## 4. 什么时候树模型打败深度学习？

虽然深度学习在图像、文本、语音等领域取得了巨大成功，但在许多场景下，树模型（尤其是 GBDT / XGBoost / LightGBM）仍然是最优选择：

### 4.1 树模型的优势

| 场景 | 为什么树模型更好 |
|:---|:---|
| **表格数据（Tabular Data）** | 表格数据通常包含混合类型（数值 + 类别），树模型天然处理非线性关系和特征交互 |
| **小样本数据** | 深度学习需要大量数据，树模型在几百到几万样本上就能表现很好 |
| **可解释性** | 特征重要性、SHAP 值、树结构可视化——树模型的决策过程透明可解释 |
| **训练效率** | 在 CPU 上即可快速训练，不需要 GPU |
| **无需特征缩放** | 树模型基于阈值分裂，不受特征尺度影响 |

### 4.2 典型的"树模型优先"场景

```
Kaggle 表格数据竞赛 → 90% 以上冠军方案使用 XGBoost / LightGBM / CatBoost
金融风控评分卡  → 逻辑回归（可解释性）或 GBDT（精度）
推荐系统 CTR 预估 → 深度学习（大规模）或 GBDT（中小规模）
医疗诊断辅助   → 树模型（可解释性要求高）
工业传感器异常检测 → 孤立森林（Isolation Forest，基于树的异常检测）
```

### 4.3 什么时候换深度学习？

- 数据量极大（百万级以上）
- 数据具有空间结构（图像、视频）
- 数据具有时序依赖（文本、语音、时间序列长程依赖）
- 需要端到端表示学习（无需手动特征工程）

---

## 总结

```
决策树 (Decision Tree)
  ├─ 单个模型，可解释但易过拟合
  ├─ 分裂标准：信息增益 / Gini 不纯度
  └─ 剪枝控制复杂度

随机森林 (Random Forest)
  ├─ Bagging + 特征随机性
  ├─ 降低方差，不增偏差
  ├─ OOB 误差天然验证
  └─ 适合中等规模表格数据

GBDT / XGBoost
  ├─ Boosting：串行纠正残差
  ├─ XGBoost：二阶梯度 + 正则化 + 近似分裂
  ├─ 精度通常最高（Kaggle 之王）
  └─ 适合结构化/表格数据
```

| 模型 | 偏差 | 方差 | 可解释性 | 训练速度 | 精度 |
|:---|:---:|:---:|:---:|:---:|:---:|
| 单棵决策树 | 低 | **高** | ★★★★★ | ★★★★★ | ★★★ |
| 随机森林 | 低 | 中 | ★★★★ | ★★★ | ★★★★ |
| XGBoost | **更低** | 中 | ★★★ | ★★★ | ★★★★★ |

> 下一章，我们将进入**无监督学习**的世界，探索聚类（K-Means, DBSCAN）和降维（PCA, t-SNE）的核心思想与实战。

## 参考文献 (References)

1. **Breiman, L. et al.** (1984). *Classification and Regression Trees*. Wadsworth. — CART 算法。
2. **Quinlan, J. R.** (1986). Induction of decision trees. *Machine Learning*, 1(1), 81–106. — ID3 算法。
3. **Freund, Y. & Schapire, R. E.** (1997). A decision-theoretic generalization of on-line learning and an application to boosting. *JCSS*, 55(1), 119–139. — AdaBoost 算法。
4. **Breiman, L.** (2001). Random forests. *Machine Learning*, 45(1), 5–32. — 随机森林。
5. **Chen, T. & Guestrin, C.** (2016). XGBoost: A scalable tree boosting system. *KDD*, 785–794. — XGBoost。

<!-- 演算盒审查完成: 无需 -->
