import React, { useEffect, useState } from 'react';
import InputNumber from '../sharedComponents/InputNumber'
import SelectCustom from '../sharedComponents/SelectCustom';
import style from './SelectRepeatMode.module.scss'

const dailyOptions = [
    { value: 1, label: 'Repeat every day' },
    { value: 2, label: 'Repeat every Two days' },
    { value: 3, label: 'Repeat every Three days' },
    { value: 4, label: 'Repeat every Four days' },
    { value: 5, label: 'Repeat every Five days' },
    { value: 6, label: 'Repeat every Six days' },
    { value: 7, label: 'Repeat every Week' },
    { value: 8, label: 'Custom' },
];

const weeklyOption = [
    { value: 0, label: 'Sunday' },
    { value: 1, label: 'Monday' },
    { value: 2, label: 'Tuesday' },
    { value: 3, label: 'Wednesday' },
    { value: 4, label: 'Thursday' },
    { value: 5, label: 'Friday' },
    { value: 6, label: 'Saturday' },
]

const repeatMethodValues = {
    daily: 0,
    weekly: 1,
    noRepeat: 2
}

function SelectRepeatMode({ plan, setPlan }) {
    const [selectedDaily, setSelectedDaily] = useState(plan.repeatMethod === repeatMethodValues.daily ? plan.repeat : 1);
    const [selectedWeek, setSelectedWeek] = useState(plan.repeatMethod === repeatMethodValues.weekly && plan.repeat ? plan.repeat.map(index => weeklyOption[index]) : []);

    const handleModeChange = (repeatMethod) => {
        console.log(selectedDaily, repeatMethod)
        setPlan(plan => ({
            ...plan,
            repeatMethod: repeatMethod,
            repeat: repeatMethod === repeatMethodValues.daily ? selectedDaily : repeatMethod === repeatMethodValues.weekly ? selectedWeek : 0
        }))
    };

    useEffect(() => {
        // Update plan when selectedDaily changes
        if (plan.repeatMethod === repeatMethodValues.daily) {
            setPlan(prevPlan => ({
                ...prevPlan,
                repeat: selectedDaily
            }));
        }
    }, [selectedDaily, plan.repeatMethod, setPlan]);

    useEffect(() => {
        // Update plan when selectedWeek changes
        if (plan.repeatMethod === repeatMethodValues.weekly) {
            setPlan(prevPlan => ({
                ...prevPlan,
                repeat: selectedWeek.map(day => day.value).sort()
            }));
        }
    }, [selectedWeek, plan.repeatMethod, setPlan]);

    const decreament = () => {
        setPlan(plan => ({
            ...plan,
            repeat: plan.repeat > 1 ? plan.repeat - 1 : 1
        }))
    }

    const increament = () => {
        setPlan(plan => ({
            ...plan,
            repeat: plan.repeat + 1
        }))
    }


    return (
        <div className={style.container}>
            <div className="mode-buttons">
                <button
                    className={`${style.button} ${plan.repeatMethod === repeatMethodValues.daily ? style.active : ''}`}
                    onClick={() => handleModeChange(repeatMethodValues.daily)} >
                    Daily Mode
                </button>
                <button
                    className={`${style.button} ${plan.repeatMethod === repeatMethodValues.weekly ? style.active : ''}`}
                    onClick={() => handleModeChange(repeatMethodValues.weekly)} >
                    Weekly Mode
                </button>
                <button
                    className={`${style.button} ${plan.repeatMethod === repeatMethodValues.noRepeat ? style.active : ''}`}
                    onClick={() => handleModeChange(repeatMethodValues.noRepeat)} >
                    No repeat
                </button>
            </div>
            {plan.repeatMethod === repeatMethodValues.daily &&
                <div>
                    <SelectCustom options={dailyOptions} isMulti={false}
                        onChange={selected => setSelectedDaily(selected.value)}
                        setValue={() => plan.repeat < 8 ? dailyOptions[plan.repeat - 1] : dailyOptions[7]} />
                    {selectedDaily > 7 && <div className={style.customContainer}>
                        <label className={style.label}>Custom repeat</label>
                        <InputNumber val={plan.repeat} increament={increament} decreament={decreament} />
                    </div>}
                </div>}
            {plan.repeatMethod === repeatMethodValues.weekly &&
                <SelectCustom options={weeklyOption} isMulti={true}
                    onChange={selected => setSelectedWeek(selected)}
                    setValue={() => selectedWeek} />}
        </div >
    );
}

export default SelectRepeatMode;
