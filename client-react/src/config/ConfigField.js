import { useState } from "react";
import style from './ConfigField.module.scss'

const ConfigField = ({ type, mapValues }) => {
    const [name, setName] = useState('')
    const [password, setPassword] = useState('')

    return (
        <div key={`config-field-${type}`} className={style.container}>
            <form key={`config-field-${type}-name`} className={style.fieldContainer} onSubmit={(e) => e.preventDefault()}>
                <label className={style.label}>{mapValues.name.message}</label>
                <div className={style.rightSideContainer}>

                    <div className={style.inputComtainer}>
                        <input className={style.input} type={mapValues.name.inputType} placeholder={mapValues.name.placeholder} onChange={event => setName(event.target.value)} required />
                        <mapValues.name.Icon className={style.icon} />
                    </div>
                    <a className={style.a} href={`http://192.168.4.1:80/config?${encodeURIComponent(mapValues?.name?.id)}=${encodeURIComponent(name)}`} target="_blank" rel="noopener noreferrer" >
                        Update
                    </a>
                </div>
            </form>
            <form key={`config-field-${type}-password`} className={style.fieldContainer} onSubmit={(e) => e.preventDefault()} required>
                <label className={style.label}>{mapValues.password.message}</label>
                <div className={style.rightSideContainer}>
                    <div className={style.inputComtainer}>
                        <input className={style.input} placeholder={mapValues.password.placeholder} onChange={event => setPassword(event.target.value)} />
                        <mapValues.password.Icon className={style.icon} />
                    </div>
                    <a className={style.a} href={`http://192.168.4.1:80/config?${encodeURIComponent(mapValues?.password?.id)}=${encodeURIComponent(password)}`} target="_blank" rel="noopener noreferrer" >
                        Update
                    </a>
                </div>
            </form>
        </div>
    );
}

export default ConfigField;