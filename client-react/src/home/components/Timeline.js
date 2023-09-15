// import {} from 'react-hori'
import style from './Timeline.module.scss'

const Timeline = () => {
    const plans = [{ startTime: '11:11', x: "x" }, { startTime: '11:11', x: "x" }, { startTime: '11:11', x: "x" }]
    return (
        <div className={`${style.container}`}>
            <div className={style.upContainer}>
                <ul>
                    <li className={`${style.step} ${style.step01} ${style.active}`}>
                        <div className={style.stepInner}>11:11</div>
                    </li>
                    <li className={`${style.step} ${style.step02}`}>
                        <div className={style.stepInner}>11:12</div>
                    </li>
                    <li className={`${style.step} ${style.step03}`}>
                        <div className={style.stepInner}>12:33</div>
                    </li>
                </ul>

                <div className={style.line}>
                    <div className={style.lineProgress}></div>
                </div>
            </div>

            <div className={style.contentContainer}>
                <div className={`${style.section} ${style.discovery} ${style.active}`}>
                    <div>discovery data...</div>
                </div>
                <div className={`${style.section} ${style.strategy} ${style.active}`}>
                    <div>strategy data...</div>
                </div>
                <div className={`${style.section} ${style.creative} ${style.active}`}>
                    <div>creative data...</div>
                </div>
            </div>
        </div>
    );
}

export default Timeline;