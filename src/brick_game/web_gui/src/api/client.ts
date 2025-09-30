import { Configuration, DefaultApi } from './index';

const config = new Configuration({
  basePath: '/api',
});

export const apiClient = new DefaultApi(config);