// import { useState } from 'react'
import { ref } from 'firebase/database';
import { useFirebase } from '../../FirebaseContext';
import SubmitButton from '../../sharedComponents/SubmitButton';
// import PopupDialog from '../../sharedComponents/PopupDialog';
import style from './Test.module.scss'

const Test = ({ areaData, dbPathArea }) => {
    const { updateDb, firebaseDB } = useFirebase()
    const today = new Date();
    // const [p, setP] = useState(false)

    // Handle changes in the dropdown selection
    const handleOptionChange = (event) => {
        updateDb(ref(firebaseDB, dbPathArea + '/activePlan'), parseInt(event.target.value))
    }


    return (
        <div className={style.container}>
            <SubmitButton text={areaData.isOpen ? "Close" : "Open"} onClick={() => updateDb(ref(firebaseDB, dbPathArea + '/isOpen'), !areaData.isOpen)} />
            {/* <SubmitButton text="popup" onClick={() => setP(!p)} /> */}
            {/* {p && <PopupDialog title="test" body="test" handleOpenModal={() => { }} handleCloseModal={() => { }} />} */}
            {areaData.plans && <select className={style.activePlan} id="dropdown" value={areaData.activePlan < areaData.plans.length ? areaData.activePlan : -1} onChange={handleOptionChange}>
                <option value="-1">-- Select an option --</option>
                {/* {<div>{console.log(JSON.stringify(areaData.plans))}</div>} */}
                {Object.keys(areaData.plans).map((key) => (
                    <option key={key.substring(2)} value={key.substring(2)}>{areaData.plans[key].startTime} </option>
                ))}
            </select>}

            <div className={style.openTime}>
                <label htmlFor="" className={style.label}>openTime</label>
                <input type='time' value={areaData.openTime ? areaData.openTime.split(' ')[1] : '00:00:00'} className={style.openTime}
                    onChange={event => updateDb(ref(firebaseDB, dbPathArea + '/openTime'), `${today.getDate()}.${today.getMonth() + 1}.23 ${event.target.value}`)}
                    step="1" />
            </div>

            <SubmitButton text={areaData.close ? "Undo close" : "close:false"} onClick={() => updateDb(ref(firebaseDB, dbPathArea + '/close'), false)} className={style.close} />
        </div>
    );
};

export default Test;