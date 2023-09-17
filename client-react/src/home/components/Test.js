import { ref } from 'firebase/database';
import { useFirebase } from '../../FirebaseContext';
import style from './Test.module.scss'
import SubmitButton from '../../sharedComponents/SubmitButton';

const Test = ({ areaData, dbPathArea }) => {
    const { updateDb, firebaseDB } = useFirebase()

    // Handle changes in the dropdown selection
    const handleOptionChange = (event) => {
        console.log(parseInt(event.target.value))
        updateDb(ref(firebaseDB, dbPathArea + '/activePlan'), parseInt(event.target.value))
    }


    return (
        <div className={style.container}>
            <SubmitButton text={areaData.isOpen ? "Close" : "Open"} onClick={() => updateDb(ref(firebaseDB, dbPathArea + '/isOpen'), !areaData.isOpen)} />

            {areaData.plans && <select className={style.activePlan} id="dropdown" value={areaData.activePlan} onChange={handleOptionChange}>
                <option value="-1">-- Select an option --</option>
                {areaData.plans.map((plan, index) => (
                    <option key={index} value={index}>{plan.startTime} </option>
                ))}
            </select>}

            <div className={style.openTime}>
                <label htmlFor="" className={style.label}>openTime</label>
                <input type='time' value={areaData.openTime} className={style.openTime}
                    onChange={event => updateDb(ref(firebaseDB, dbPathArea + '/openTime'), event.target.value)}
                    step="1" />
            </div>

            <SubmitButton text={areaData.close ? "Undo close" : "close:false"} onClick={() => updateDb(ref(firebaseDB, dbPathArea + '/close'), false)} className={style.close} />
        </div>
    );
};

export default Test;