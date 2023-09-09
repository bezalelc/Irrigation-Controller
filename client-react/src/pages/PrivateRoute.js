import React from 'react';
import { Navigate } from 'react-router-dom';
import { useFirebase } from '..//FirebaseContext';


function PrivateRoute({ component: Component }) {
  const { userId } = useFirebase();


  return (
    userId ? <Component /> : <Navigate to='/' />
  );
}

export default PrivateRoute;
