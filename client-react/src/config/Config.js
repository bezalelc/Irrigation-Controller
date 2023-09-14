import ConfigField from './ConfigField';
import { FaWifi } from 'react-icons/fa';
import { RiLockPasswordLine } from 'react-icons/ri';
import { BiSolidKey } from 'react-icons/bi';
import { IoMdWifi } from 'react-icons/io';
import { MdOutlineEmail } from 'react-icons/md';
import style from './Config.module.scss'

const configMap = {
    wifi: {
        name: {
            id: '0',
            message: 'Update your home WiFi name',
            placeholder: 'Home WiFi name',
            inputType: 'text',
            note: '',
            Icon: FaWifi
        },
        password: {
            id: '1',
            message: 'Update your home WiFi password',
            placeholder: 'Home WiFi Password',
            inputType: 'password',
            note: '',
            Icon: BiSolidKey
        }
    },
    ap: {
        name: {
            id: '2',
            message: 'Change the Access point name of the Irrigation controller',
            placeholder: 'Access point name',
            inputType: 'text',
            note: '',
            Icon: IoMdWifi
        },
        password: {
            id: '3',
            message: 'Change the Access point password of the Irrigation controller',
            placeholder: 'Access point password',
            inputType: 'password',
            note: '',
            Icon: BiSolidKey
        }
    },
    user: {
        name: {
            id: '4',
            message: 'Update your email',
            placeholder: 'Email',
            inputType: 'email',
            note: '',
            Icon: MdOutlineEmail
        },
        password: {
            id: '5',
            message: 'Update your password account',
            placeholder: 'Password',
            inputType: 'password',
            note: '',
            Icon: RiLockPasswordLine
        }
    }
}

const Config = () => {

    return (
        <div className={style.container}>
            <div className={style.title}>Thermometer configuration</div>
            <div className={style.instruction}>
                <b className={style.b}>instruction:</b>
                <ul>
                    <li>Torn on configuration mode in the thermometer</li>
                    <li>Search for the thermometer's hotspot in your device and connect</li>
                    <li>Update one of those fields and click Update</li>
                </ul>
            </div>
            <div className={style.configFields}>
                <ConfigField type='wifi' mapValues={configMap.wifi} />
                <hr className={style.separator} />
                <ConfigField type='ap' mapValues={configMap.ap} />
                <hr className={style.separator} />
                <ConfigField type='user' mapValues={configMap.user} />
            </div>
        </div>
    );
}

export default Config;