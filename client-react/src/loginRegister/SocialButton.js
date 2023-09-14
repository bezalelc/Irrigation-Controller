
import { GoogleAuthProvider, signInWithPopup } from 'firebase/auth'
import { useFirebase } from "../FirebaseContext";
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faGoogle } from '@fortawesome/free-brands-svg-icons';
import style from './SocialButton.module.scss'

const SocialButton = () => {
    const { firebaseAuth } = useFirebase()

    const loginWithGoogleHandler = async () => {
        const googleAuthProvider = new GoogleAuthProvider();

        signInWithPopup(firebaseAuth, googleAuthProvider)
            .then(() => {
                // This gives you a Google Access Token. You can use it to access the Google API.
                // const credential = GoogleAuthProvider.credentialFromResult(result);
                // const token = credential.accessToken;
                // // The signed-in user info.
                // const user = result.user;
                // IdP data available using getAdditionalUserInfo(result)
                // ...
            }).catch((error) => {
                const errorCode = error.code;
                const errorMessage = error.message;
                console.log(errorCode)
                console.log(errorMessage)
            });
    }

    return (
        <div className={style.container}>
            <button className={style.button} onClick={loginWithGoogleHandler}><FontAwesomeIcon icon={faGoogle} className={style.googleIcon} />Continue with Google</button>
        </div>
    );
}

export default SocialButton;