## libjson Test
 
#### 実行
```
LD_LIBRARY_PATH=../ ./main
```
```
{"key1":"value1","key2":"value2","key3":"value3","key4":"value4","key5":"value5","in_object_1":{"number1":1204}}
```

#### jq
```
LD_LIBRARY_PATH=../ ./main | jq .
```
``` response.json
{
  "key1": "value1",
  "key2": "value2",
  "key3": "value3",
  "key4": "value4",
  "key5": "value5",
  "in_object_1": {
    "number1": 1204
  }
}
```
