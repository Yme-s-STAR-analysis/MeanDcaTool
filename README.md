# Mean DCA tool

version: 2.1

author: yghuang

data: 1.4.2024

## Patch note

03.04.2024 by yghuang (v2.1.1):

> Fix a bug: additional commas in ReadParams() are removed

01.04.2024 by yghuang (v2.1):

> DCA cuts are embedded in a parameter header

29.11.2023 by yghuang (v2.0.1):

> Fix a fatal bug: IsBadDcaZ/XYEvent() was returning an incorrect boolean.

29.11.2023 by yghuang (v2.0):

> New version:
>
>> can directly read from picoDst and return good or bad by specifying the fit functions in advance
>>
>> When one only wants to know the mean DCAz/xy, use Make(pico), and get mDCAz/xy
>>
>> When one wants to know the event is good or bad, use IsBadMeanDcaZEvent(pico) or IsBadMeanDcaXYEvent(pico)
