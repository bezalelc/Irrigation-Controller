import { useState } from "react";
import EditPlan from "./EditPlan";

const Plan = ({ planId, plan, updatePlansSorted }) => {
    const [editPlanPopUp, setEditPlanPopup] = useState(false)

    return (
        <>
            <div>start time: {plan.startTime}</div>
            <div>last time: {plan.lastTime}</div>
            <div>duration: {plan.duration}</div>
            <div>repeat method: {plan.repeatMethod}</div>
            <div>repeat: {plan.repeat}  id:{planId}</div>
            <button onClick={() => updatePlansSorted("remove", planId)}>Remove plan</button>
            <button onClick={() => setEditPlanPopup(true)}>Edit plan</button>
            {editPlanPopUp && <EditPlan plan={plan} planId={planId} updatePlansSorted={updatePlansSorted} setAddPlanPopup={setEditPlanPopup} />}
        </>
    );
}

export default Plan;