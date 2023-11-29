# Mean DCA tool

version: 2.0

author: yghuang

data: 29.11.2023

## Patch note

29.11.2023 by yghuang (2.0):

> New version:
>
>> can directly read from picoDst and return good or bad by specifying the fit functions in advance
>>
>> When one only wants to know the mean DCAz/xy, use Make(pico), and get mDCAz/xy
>>
>> When one wants to know the event is good or bad, use IsBadMeanDcaZEvent(pico) or IsBadMeanDcaXYEvent(pico)