# wait
Atomically waits until the value of this atomic object has changed. If order is either `MEMORY_ORDER_RELEASE` or `MEMORY_ORDER_ACQ_REL`, the behavior is undefined.

```nvgt
void wait(T old, memory_order order = MEMORY_ORDER_SEQ_CST);
```

## Parameters:
* `T old`: The old (current) value of this atomic object as of the time of this call. This function will wait until this atomic object no longer contains this value.
* `memory_order order`: memory order constraints to enforce.

## Remarks:
This function is available on all atomic types.

Within the above function signature, `T` is used as a placeholder for the actual type. For example, if this object is an `atomic_int`, then `T` SHALL be `int`.

This function is guaranteed to return only when the value has changed, even if the underlying implementation unblocks spuriously.
