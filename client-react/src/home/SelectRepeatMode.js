import React, { useEffect, useState } from 'react';
import Select from 'react-select';
import InputNumber from '../sharedComponents/InputNumber'
import style from './SelectRepeatMode.module.scss'

const selectStyle = {
    container: (provided, state) => ({
        // Style for the container that wraps the entire Select component
        ...provided,
        boxShadow: 'none',
    }),

    control: (provided, state) => ({
        // Style for the control container (the outer container)
        ...provided,
        border: "2px solid grey",
        borderColor: state.isFocused ? '#1c96ee97' : 'gray',
        borderRadius: '10px',
        backgroundColor: 'rgba(0, 0, 0, 0.2)',
    }),

    valueContainer: (provided, state) => ({
        // Style for the value container (where selected values are displayed)
        ...provided,
        padding: '5px 5px',
    }),

    menu: (provided, state) => ({
        // Style for the dropdown menu container
        ...provided,
        backgroundColor: 'rgba(0, 0, 0, 0.2)',
        // color:"red",
    }),

    option: (provided, state) => ({
        // Style for individual dropdown options
        ...provided,
        backgroundColor: state.isFocused ? 'rgba(0, 0, 0, 0.2)' : 'rgba(0, 0, 0, 0.7)',
        backdropFilter: 'blur(20px)',
        // backgroundColor: 'rgba(0, 0, 0, 0.2)',
        color: "#d7e8ed",
        border: "1px black solid",
    }),

    singleValue: (provided, state) => ({
        // Style for the single selected value
        ...provided,
        color: 'black',
        fontSize: "1.5rem",
        fontWeight: "500",
        backgroundColor: 'rgba(0, 0, 0, 0.2)',
        padding: '5px 5px',
        borderRadius: "10px",
    }),

    multiValue: (provided, state) => ({
        // Style for a selected value in a multi-select
        ...provided,
        backgroundColor: 'rgba(0, 0, 0, 0.2)',
        borderRadius: "10px",
        // backgroundColor: state.isFocused ? "red" :""
    }),

    multiValueLabel: (provided, state) => ({
        // Style for the label of a selected value in a multi-select
        ...provided,
        fontSize: "1.5rem",
        fontWeight: "500",
        color: "black",
    }),

    multiValueRemove: (provided, state) => ({
        // Style for the remove button of a selected value in a multi-select
        ...provided,
        color: 'gray',
    }),

    indicatorSeparator: (provided, state) => ({
        // Style for the separator between indicators (e.g., dropdown arrow and clear button)
        ...provided,
        backgroundColor: 'gray',
        color: "red",
    }),

    dropdownIndicator: (provided, state) => ({
        // Style for the dropdown indicator (the dropdown arrow)
        ...provided,
        // color: state.isHover ? "black" : "gray",
    }),

    placholder: (provided) => ({
        ...provided,
        // color: "black"
    }),

    clearIndicator: (provided, state) => ({
        ...provided,
        // color: state.isFocused ? "red" : "gray",
        // backgroundColor: state.isFocused ? "rgba(247, 151, 138, 0.451)" : "rgba(0, 0, 0, 0)",
        borderRadius: "10px",
        marginRight: "5px",
    }),

};

const dailyOptions = [
    { value: 0, label: 'No repeat' },
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

function SelectRepeatMode({ plan, setPlan }) {
    const [selectedDaily, setSelectedDaily] = useState(plan.repeatMethod === 'daily' ? plan.repeat : 1);
    const [selectedWeek, setSelectedWeek] = useState(plan.repeatMethod === 'weekly' && plan.repeat ? plan.repeat.map(index => weeklyOption[index]) : []);

    const handleModeChange = (repeatMethod) => {
        setPlan(plan => ({
            ...plan,
            repeatMethod: repeatMethod,
            repeat: repeatMethod === 'daily' ? selectedDaily : repeatMethod === 'weekly' ? selectedWeek : 0
        }))
    };

    useEffect(() => {
        // Update plan when selectedDaily changes
        if (plan.repeatMethod === 'daily') {
            setPlan(prevPlan => ({
                ...prevPlan,
                repeat: selectedDaily
            }));
        }
    }, [selectedDaily, plan.repeatMethod, setPlan]);

    useEffect(() => {
        // Update plan when selectedWeek changes
        if (plan.repeatMethod === 'weekly') {
            setPlan(prevPlan => ({
                ...prevPlan,
                repeat: selectedWeek.map(day => day.value).sort()
            }));
        }
    }, [selectedWeek, plan.repeatMethod, setPlan]);

    const decreament = () => {
        setPlan(plan => ({
            ...plan,
            repeat: plan.repeat > 0 ? plan.repeat - 1 : 0
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
                    className={`${style.button} ${plan.repeatMethod === 'daily' ? style.active : ''}`}
                    onClick={() => handleModeChange('daily')} >
                    Daily Mode
                </button>
                <button
                    className={`${style.button} ${plan.repeatMethod === 'weekly' ? style.active : ''}`}
                    onClick={() => handleModeChange('weekly')} >
                    Weekly Mode
                </button>
                <button
                    className={`${style.button} ${plan.repeatMethod === 'noRepeat' ? style.active : ''}`}
                    onClick={() => handleModeChange('noRepeat')} >
                    No repeat
                </button>
            </div>
            {plan.repeatMethod === 'daily' &&
                <div>
                    <Select styles={selectStyle} options={dailyOptions} isMulti={false} value={plan.repeat < 8 ? dailyOptions[plan.repeat] : dailyOptions[8]} onChange={selected => setSelectedDaily(selected.value)} isSearchable />
                    {selectedDaily > 7 && <div className={style.customContainer}>
                        <label className={style.label}>Custom repeat</label>
                        <InputNumber val={plan.repeat} increament={increament} decreament={decreament} />
                    </div>}
                </div>}
            {plan.repeatMethod === 'weekly' &&
                <Select value={selectedWeek} styles={selectStyle} options={weeklyOption} isMulti onChange={selected => setSelectedWeek(selected)} isSearchable />}
        </div >
    );
}

export default SelectRepeatMode;
