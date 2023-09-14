import { useState } from "react";
import { ref } from "firebase/database";
import { useFirebase } from "../FirebaseContext";
import InputNumber from "../sharedComponents/InputNumber";
import style from './Setting.module.scss'

const Setting = () => {
    const { firebaseDB, updateDb, data, dbPath } = useFirebase()
    const [updateDelaySec, setUpdateDelaySec] = useState(data.setting.updateDelaySec)
    const [maxTaps, setMaxTaps] = useState(data.setting.maxTaps)
    const settingPath = dbPath + '/setting'
    const updateDelaySecRef = ref(firebaseDB, settingPath + '/updateDelaySec')
    const maxTapsRef = ref(firebaseDB, settingPath + '/maxTaps')

    return (
        <div className={style.container}>
            <div className={style.title}>Setting</div>
            <div className={style.valueContainer}>
                <label className={style.label}>Delay sec </label>
                <div className={style.rightSideContainer}>
                    <InputNumber val={updateDelaySec} increament={() => setUpdateDelaySec(prev => prev + 1)} decreament={() => setUpdateDelaySec(prev => prev > 0 ? prev - 1 : 0)} />
                    <button className={style.button} onClick={() => { updateDb(updateDelaySecRef, updateDelaySec) }}>Update</button>
                </div>
            </div >
            <hr className={style.separator} />
            <div className={style.valueContainer}>
                <label className={style.label}>Max taps </label>
                <div className={style.rightSideContainer}>
                    <InputNumber val={maxTaps} increament={() => setMaxTaps(prev => prev + 1)} decreament={() => setMaxTaps(prev => prev > 1 ? prev - 1 : 1)} />
                    <button className={style.button} onClick={() => { updateDb(maxTapsRef, maxTaps) }}>Update</button>
                </div>
            </div>
        </div>
    );
}

export default Setting;