import { useState } from "react";
import { ref } from "firebase/database";
import { useFirebase } from "../FirebaseContext";
import InputNumber from "../sharedComponents/InputNumber";
import SelectCustom from "../sharedComponents/SelectCustom";
import style from './Setting.module.scss'

// const updateMethod =
//     [
//         continuous,
//         everyMinute,
//         every5minute,
//         every30minute,
//         everyHour,
//         everyDay
//     ];

const updateMethodOptions = [
    { value: 0, label: 'Continuous' },
    { value: 1, label: 'Every minute' },
    { value: 2, label: 'Every 5 minute' },
    { value: 3, label: 'Every 30 minute' },
    { value: 4, label: 'Every hour' },
    { value: 5, label: 'Every day' }
]

const maxTapsConnected = 32;

const Setting = () => {
    const { firebaseDB, updateDb, data, dbPath } = useFirebase()
    const [maxTaps, setMaxTaps] = useState(data.setting.maxTaps)
    const settingPath = dbPath + '/setting'
    const updateMethodRef = ref(firebaseDB, settingPath + '/updateMethod')
    const maxTapsRef = ref(firebaseDB, settingPath + '/maxTaps')

    return (
        <div className={style.container}>
            <div className={style.title}>Setting</div>
            <div className={style.settingContainer}>
                <div className={style.valueContainer}>
                    <label className={style.label}>Delay method </label>
                    <SelectCustom isMulti={false} options={updateMethodOptions} onChange={selected => { updateDb(updateMethodRef, selected.value) }} setValue={() => updateMethodOptions[data.setting.updateMethod]} />
                </div >
                <hr className={style.separator} />
                <div className={style.valueContainer}>
                    <label className={style.label}>Max taps </label>
                    <div className={style.rightSideContainer}>
                        <InputNumber val={maxTaps} increament={() => setMaxTaps(prev => prev < maxTapsConnected ? prev + 1 : prev)} decreament={() => setMaxTaps(prev => prev > 1 ? prev - 1 : 1)} />
                        <button className={style.button} onClick={() => updateDb(maxTapsRef, maxTaps)}>Update</button>
                    </div>
                </div>
            </div>
        </div>
    );
}

export default Setting;