import { useState } from "react";
import { ref } from "firebase/database";
import { BsPencilFill } from 'react-icons/bs';
import AddPlan from "./EditPlan";
import Plan from "./Plan";
import Timer from "./Timer";
import { useFirebase } from "../FirebaseContext";
import CircularProgressBar from "./components/CircularProgressBar";
import Switch from "./Switch";
import style from './Area.module.scss'

const Area = ({ id, areaData, dbPathAreas }) => {
    const dbPathArea = dbPathAreas + `/${id}`
    const { updateDb, firebaseDB } = useFirebase()
    const plansRef = ref(firebaseDB, dbPathArea + '/plans')
    const nameRef = ref(firebaseDB, dbPathArea + '/name')
    const [name, setName] = useState(areaData.name)
    const [addPlanPopUp, setAddPlanPopup] = useState(false)
    const manualOpenRef = ref(firebaseDB, dbPathArea + '/manualOpen')

    const updatePlansSorted = (operation, plan) => {
        if (!areaData.plans || !areaData.plans.length) {
            areaData.plans = plan ? [plan] : false
        } else {
            if (operation === 'add') {
                areaData.plans.push(plan)
            } else if (operation === 'remove') {
                areaData.plans.splice(plan, 1)
            }
            // else if (operation === 'update') {
            //     areaData.plans.splice(plan, 1)
            // }
            areaData.plans = areaData.plans.sort((a, b) => {
                const startTimeA = a.startTime;
                const startTimeB = b.startTime;
                return startTimeA.localeCompare(startTimeB);
            });
        }
        updateDb(plansRef, areaData.plans)
    }

    const manualButton = () => {
        updateDb(manualOpenRef, !areaData.manualOpen)
        // console.log('areaData.manualOpen', areaData.manualOpen, ', areaData.isOpen', areaData.isOpen)
    }

    return (
        <div className={style.container}>
            <div className={style.setting}>
                <div className={style.setNameContainer}>
                    <input type="text" value={areaData.name} className={style.input} onChange={event => setName(event.target.value)} onBlur={updateDb(nameRef, name)} />
                    <BsPencilFill className={style.icon} />
                </div>
                <button className={style.addPlan} onClick={() => setAddPlanPopup(prev => !prev)}>Add plan</button>
                <div className={style.switchContainer}>
                    <label className={style.manualOpenLabel}>Manual open</label>
                    <Switch manualOpen={areaData.manualOpen} dbPathArea={dbPathArea} />
                </div>
                <div className={style.switchContainer}>
                    <label className={style.fertilizerLabel}>Set as Fertilizer</label>
                    <Switch manualOpen={areaData.manualOpen} dbPathArea={dbPathArea} />
                </div>
            </div>
            {/* <CircularProgressBar />
            <button className={`${style.manualButton} ${areaData.manualOpen ? style.manualButtonOpen : ''}`} onClick={manualButton}>{areaData.manualOpen ? 'On' : areaData.isOpen ? 'Off' : 'Waiting...'}</button>
            {areaData.manualOpen && !areaData.isOpen && <div>Waiting...</div>}
            {areaData.isOpen && <Timer />}
            <div>now {areaData.isOpen ? "open" : "closed"}</div>
            {addPlanPopUp && <AddPlan updatePlansSorted={updatePlansSorted} setAddPlanPopup={setAddPlanPopup} />}
            {areaData.plans &&
                <div>
                <div>plans</div>
                {Object.entries(areaData.plans).map(([id, plan]) => (
                    <Plan key={id} planId={id} plan={plan} updatePlansSorted={updatePlansSorted} />
                    ))}
                </div>} */}
            {addPlanPopUp && <AddPlan updatePlansSorted={updatePlansSorted} setAddPlanPopup={setAddPlanPopup} />}
        </div>
    );
}

export default Area;