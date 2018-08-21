bitcoin难度调整分析
===================================

https://github.com/bitcoin/bitcoin

概述
-----------------------------------
nBits难度调整值统一由`GetNextWorkRequired`给出，前2016块难度均为最低难度`1`，难度值调整受前`2016`块总的出块时间以及当前块难度决定。

难度值计算
-----------------------------------

* **每2016块调整一次难度值:**
> 产生2016块所需标准时间
```math
nPowTargetTime = 14 * 24 * 60 * 60 = 2016 * 60 * 10 = 1209600
```
按照每十分钟出一个块，两周会调整一次难度值。

* **从以上可得出，前`2015`块难度值均为`1`：**
> **nActualTime**: 实际出2016块所花时间
```math
nActualTime = blockTime_{2016 \times (n+1)} - blockTime_{2016 \times n}, n \geq 0
```

* **限制`nActualTime`变化范围:**
```math
nActualTime \in [\frac{1}{4}, 4] \times nPowTargetTime
```
对实际时间差做了上下浮动限制，不得超出标准的1/4或4倍。
* **经计算后的难度值变化范围:**
> **bnNew_old**: 上一个2016块难度值
```math
bnNew = bnNew_{old} \times \frac{nActualTime}{nPowTargetTime}
```

* **最大难度上限：**
```math
bnNew \leq UintToArith256(params \cdot powLimit)
```
难度值上限为0x00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff

* **经转换后的难度值:**
```math
bnNew.GetCompact()
```