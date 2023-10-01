import { useState } from "react";
import { ref } from "firebase/database";
import { BsPencilFill } from 'react-icons/bs';
import { useFirebase } from "../FirebaseContext";
import AddPlan from "./EditPlan";
import Plan from "./Plan";
import Timer from "./Timer";
import CircularProgressBar from "./components/CircularProgressBar";
import Switch from "./Switch";
import Test from "./components/Test";
import ManualButton from "./components/ManualButton";
import style from './Area.module.scss'
import SubmitButton from "../sharedComponents/SubmitButton";

const Area = ({ id, areaData, dbPathAreas }) => {
    const { updateDb, firebaseDB, data } = useFirebase()
    const dbPathArea = dbPathAreas + `/${id}`
    const plansRef = ref(firebaseDB, dbPathArea + '/plans')
    const nameRef = ref(firebaseDB, dbPathArea + '/name')
    const manualOpenRef = ref(firebaseDB, dbPathArea + '/manualOpen')
    const fertilizerRef = ref(firebaseDB, dbPathArea + '/fertilizer')
    const [name, setName] = useState(areaData.name)
    const [addPlanPopUp, setAddPlanPopup] = useState(false)

    // Function to check if a new plan overlaps with existing plans
    function isPlanOverlapping(newPlan) {
        const newStartTime = new Date(`2000-01-01T${newPlan.startTime}`);
        const newEndTime = new Date(newStartTime.getTime() + newPlan.duration * 60000);

        for (const existingPlan of areaData.plans) {
            const existingStartTime = new Date(`2000-01-01T${existingPlan.startTime}`);
            const existingEndTime = new Date(existingStartTime.getTime() + existingPlan.duration * 60000);

            // Check if there is an overlap
            if (
                (newStartTime >= existingStartTime && newStartTime < existingEndTime) || // New plan starts within existing plan
                (newEndTime > existingStartTime && newEndTime <= existingEndTime) || // New plan ends within existing plan
                (newStartTime <= existingStartTime && newEndTime >= existingEndTime) // New plan completely overlaps existing plan
            ) {
                return true; // Overlap detected
            }
        }

        return false; // No overlap detected
    }

    const updatePlansSorted = (operation, plan) => {
        if (!areaData.plans || !areaData.plans.length) {
            areaData.plans = plan ? [plan] : false
        } else {
            if (operation === 'add' || operation === 'update') {
                if (isPlanOverlapping(plan)) {
                    return false;
                }
            }

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

    const deleteArea = () => {
        updateDb(ref(firebaseDB, dbPathAreas), data.areas.filter((_, index) => parseInt(index) !== parseInt(id)))
    }

    return (
        <div className={style.container}>
            <div className={style.containerNav}>
                <div className={style.setting}>
                    <div className={style.firstRowContainer}>
                        <div className={style.setNameContainer} info="Edit Name">
                            <input type="text" value={name} className={style.input}
                                onChange={event => { setName(event.target.value) }}
                                onBlur={() => { updateDb(nameRef, name) }} />
                            <BsPencilFill className={style.icon} />
                        </div>
                        <SubmitButton text="Add plan" onClick={() => setAddPlanPopup(prev => !prev)} className={style.addPlan} />
                    </div>
                    <div className={style.switchContainer}>
                        <label className={style.switchLabel}>Manual open</label>
                        <Switch value={areaData.manualOpen} dbRef={manualOpenRef} />
                    </div>
                    <div className={style.switchContainer}>
                        <label className={style.switchLabel}>Set as Fertilizer</label>
                        <Switch value={areaData.fertilizer} dbRef={fertilizerRef} />
                    </div>
                    <SubmitButton text="Delete Area" onClick={deleteArea} className={style.deleteContainer} />
                </div>

                {areaData.activePlan > -1 && areaData.isOpen && <div className={style.status}>
                    <CircularProgressBar plan={areaData.plans[areaData.activePlan]} area={areaData} />
                </div>}

                <div className={style.manualButtonContainer}>
                    <ManualButton isOpen={areaData.isOpen} close={areaData.close} manualOpen={areaData.manualOpen} dbPathArea={dbPathArea} />
                    {(areaData.isOpen || areaData.manualOpen) && <Timer openTime={areaData.openTime} isOpen={areaData.isOpen} />}
                </div>

                <Test areaData={areaData} dbPathArea={dbPathArea} />
            </div>
            {areaData.plans &&
                <div className={style.containerPlans}>
                    {Object.entries(areaData.plans).map(([id, plan]) => (
                        <Plan key={id} planId={id} plan={plan} updatePlansSorted={updatePlansSorted}
                            isActive={areaData.activePlan === parseInt(id)} />
                    ))}
                </div>}
            {addPlanPopUp && <AddPlan updatePlansSorted={updatePlansSorted} setAddPlanPopup={setAddPlanPopup} />}
        </div>
    );
}

export default Area;