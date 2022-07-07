from math import ceil


TIM_CLK = 64e6
PSC_BIT_SIZE = 16

targetFreq = 1000
targetDuty = 40

div_req = ceil(TIM_CLK/targetFreq)
psc_min = ceil(div_req/(pow(2,PSC_BIT_SIZE)))
psc_code = psc_min - 1 # code is 1 less than the prescaler value

arr_code = round(TIM_CLK/targetFreq/(psc_code+1) - 1)
pwm_code = round((targetDuty/100) * arr_code)

print(f'Timer Clock Source Freq: {TIM_CLK/1e6} MHz')
print(f'Target Frequency: {targetFreq/1e3} kHz')
print(f'Target Duty: {targetDuty} %')
print(f'Prescaler Value (min): {psc_min}')
print('_____Codes_____')
print(f'div_req: {div_req}')
print(f'psc_min: {psc_min}')
print(f'Prescaler Code: {psc_code}')
print(f'ARR Code: {arr_code}')
print(f'PWM Duty Code: {pwm_code}')

