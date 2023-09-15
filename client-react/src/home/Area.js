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
import Test from "./components/Test";

const Area = ({ id, areaData, dbPathAreas }) => {
    const { updateDb, firebaseDB } = useFirebase()
    const dbPathArea = dbPathAreas + `/${id}`
    const plansRef = ref(firebaseDB, dbPathArea + '/plans')
    const nameRef = ref(firebaseDB, dbPathArea + '/name')
    const manualOpenRef = ref(firebaseDB, dbPathArea + '/manualOpen')
    const fertilizeRef = ref(firebaseDB, dbPathArea + '/fertilize')

    const [name, setName] = useState(areaData.name)
    const [addPlanPopUp, setAddPlanPopup] = useState(false)

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

    return (
        <div className={style.container}>
            <div className={style.containerNav}>
                <div className={style.setting}>
                    <div className={style.firstRowContainer}>
                        <div className={style.setNameContainer} info="Edit Name">
                            <input type="text" value={areaData.name} className={style.input} onChange={event => setName(event.target.value)} onBlur={updateDb(nameRef, name)} />
                            <BsPencilFill className={style.icon} />
                        </div>
                        <button className={style.addPlan} onClick={() => setAddPlanPopup(prev => !prev)}>Add plan</button>
                    </div>
                    <div className={style.switchContainer}>
                        <label className={style.switchLabel}>Manual open</label>
                        <Switch manualOpen={areaData.manualOpen} dbRef={manualOpenRef} />
                    </div>
                    <div className={style.switchContainer}>
                        <label className={style.switchLabel}>Set as Fertilizer</label>
                        <Switch manualOpen={areaData.fertilizer} dbRef={fertilizeRef} />
                    </div>
                </div>

                <div className={style.status}>
                    <CircularProgressBar />
                </div>

                <div className={style.manualButtonContainer}>
                    <button className={`${style.manualButton} ${areaData.manualOpen ? style.manualButtonOpen : ''}`} onClick={() => { }}>{areaData.manualOpen ? 'On' : areaData.isOpen ? 'Off' : 'Waiting...'}</button>
                    {!areaData.isOpen && <Timer />}
                </div>

                <Test areaData={areaData} dbPathArea={dbPathArea} />
            </div>
            {areaData.plans &&
                <div className={style.containerPlans}>
                    {Object.entries(areaData.plans).map(([id, plan]) => (
                        <Plan key={id} planId={id} plan={plan} updatePlansSorted={updatePlansSorted} />
                    ))}
                </div>}
            {/*             
            {areaData.manualOpen && !areaData.isOpen && <div>Waiting...</div>}
            <div>now {areaData.isOpen ? "open" : "closed"}</div>
            {addPlanPopUp && <AddPlan updatePlansSorted={updatePlansSorted} setAddPlanPopup={setAddPlanPopup} />}
                 */}
            {/* <Timeline /> */}
            {addPlanPopUp && <AddPlan updatePlansSorted={updatePlansSorted} setAddPlanPopup={setAddPlanPopup} />}
        </div>
    );
}

export default Area;